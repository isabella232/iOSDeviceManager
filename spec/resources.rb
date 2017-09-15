
module IDM
  require "singleton"
  require "run_loop"

  def self.exit_status(key)
    case key
    when :success
      0
    when :failure
      1
    when :false
      2
    when :missing_arg
      3
    when :invalid_arg
      4
    when :internal_error
      5
    when :invalid_command
      6
    when :invalid_flag
      7
    when :device_not_found
      8
    when :no_signing_identity
      9
    else
      raise ArgumentError, "Expected a valid key; found #{key}"
    end
  end

  def self.shell(args)
    cmd = [Resources.instance.idm] + args
    RunLoop::Shell.run_shell_command(cmd, {:log_cmd => true})
  end

  class Resources
    include Singleton

    def project_dir
      @project_dir ||= File.expand_path(File.join(File.dirname(__FILE__), ".."))
    end

    def resources_dir
      @resources_dir ||= File.join(project_dir, "Tests", "Resources")
    end

    def idm
      @idm ||= File.join(project_dir, "Products", "iOSDeviceManager")
    end

    def tmp_dir(subdir=nil)
      @tmp_dir ||= File.join(project_dir, "tmp")
      if subdir
        dir = File.join(tmp_dir, subdir)
      else
        dir = @tmp_dir
      end
      FileUtils.mkdir_p(dir)
      dir
    end

    def xcode
      @xcode ||= RunLoop::Xcode.new
    end

    def simctl
      @simctl ||= RunLoop::Simctl.new
    end

    def default_simulator
      @default_simulator ||= simctl.simulators.detect do |sim|
        sim.instruments_identifier(xcode) == RunLoop::Core.default_simulator
      end
    end

    def instruments
      @instruments ||= RunLoop::Instruments.new
    end

    def test_app(type)
      @test_app_hash ||= Hash.new
      return @test_app_hash[type] if @test_app_hash[type]

      case type
      when :arm
        app = File.join(resources_dir, "arm", "TestApp.app")
      when :x86
        app = File.join(resources_dir, "sim", "TestApp.app")
      when :ipa
        app = File.join(resources_dir, "arm", "TestApp.ipa")
      else
        raise ArgumentError, "Expected :arm, :x86, or :ipa, found: #{type}"
      end

      @test_app_hash[type] = app
      app
    end

    def device_compatible_with_xcode?(device, xcode)
      device_version = device.version
      xcode_version = xcode.version

      if device_version.major < (xcode_version.major + 2)
        return true
      end

      if device_version.major == (xcode_version.major + 2)
        return device_version.minor <= xcode_version.minor
      end

     false
    end

    def default_physical_device
      if @default_physical_device == ""
        return nil
      elsif @default_physical_device
        return @default_physical_device
      end

      devices = instruments.physical_devices.select do |device|
        device_compatible_with_xcode?(device, xcode)
      end

      if devices.empty?
        @default_physical_device = ""
      else
        @default_physical_device = devices.first
      end
    end

    def physical_device_attached?
      default_physical_device != ""
    end
  end
end