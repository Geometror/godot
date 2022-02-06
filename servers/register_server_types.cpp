/*************************************************************************/
/*  register_server_types.cpp                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "register_server_types.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "audio_server.h"
#include "camera_server.h"
#include "core/extension/native_extension_manager.h"
#include "display_server.h"
#include "navigation_server_2d.h"
#include "navigation_server_3d.h"
#include "physics_2d/godot_physics_server_2d.h"
#include "physics_3d/godot_physics_server_3d.h"
#include "physics_server_2d.h"
#include "physics_server_2d_wrap_mt.h"
#include "physics_server_3d.h"
#include "physics_server_3d_wrap_mt.h"
#include "rendering_server.h"
#include "servers/rendering/shader_types.h"
#include "text_server.h"
#include "xr_server.h"
#include "core/extension/native_extension.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/os/memory.h"
#include "core/os/os.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class AudioEffect;
class AudioEffectAmplify;
class AudioEffectBandLimitFilter;
class AudioEffectBandPassFilter;
class AudioEffectCapture;
class AudioEffectChorus;
class AudioEffectCompressor;
class AudioEffectDelay;
class AudioEffectDistortion;
class AudioEffectEQ10;
class AudioEffectEQ21;
class AudioEffectEQ6;
class AudioEffectEQ;
class AudioEffectFilter;
class AudioEffectHighPassFilter;
class AudioEffectHighShelfFilter;
class AudioEffectInstance;
class AudioEffectLimiter;
class AudioEffectLowPassFilter;
class AudioEffectLowShelfFilter;
class AudioEffectNotchFilter;
class AudioEffectPanner;
class AudioEffectPhaser;
class AudioEffectPitchShift;
class AudioEffectRecord;
class AudioEffectReverb;
class AudioEffectSpectrumAnalyzer;
class AudioEffectSpectrumAnalyzerInstance;
class AudioEffectStereoEnhance;
class AudioStream;
class AudioStreamGenerator;
class AudioStreamGeneratorPlayback;
class AudioStreamMicrophone;
class AudioStreamPlayback;
class AudioStreamPlaybackResampled;
class AudioStreamRandomPitch;
class CameraFeed;
class RDAttachmentFormat;
class RDFramebufferPass;
class RDPipelineColorBlendState;
class RDPipelineColorBlendStateAttachment;
class RDPipelineDepthStencilState;
class RDPipelineMultisampleState;
class RDPipelineRasterizationState;
class RDPipelineSpecializationConstant;
class RDSamplerState;
class RDShaderFile;
class RDShaderSPIRV;
class RDShaderSource;
class RDTextureFormat;
class RDTextureView;
class RDUniform;
class RDVertexAttribute;
class RenderingDevice;
class TextServerExtension;
class XRInterface;
class XRInterfaceExtension;
class XRPose;
class XRPositionalTracker;

ShaderTypes *shader_types = nullptr;

PhysicsServer3D *_createGodotPhysics3DCallback() {
	bool using_threads = GLOBAL_GET("physics/3d/run_on_separate_thread");

	PhysicsServer3D *physics_server = memnew(GodotPhysicsServer3D(using_threads));

	return memnew(PhysicsServer3DWrapMT(physics_server, using_threads));
}

PhysicsServer2D *_createGodotPhysics2DCallback() {
	bool using_threads = GLOBAL_GET("physics/2d/run_on_separate_thread");

	PhysicsServer2D *physics_server = memnew(GodotPhysicsServer2D(using_threads));

	return memnew(PhysicsServer2DWrapMT(physics_server, using_threads));
}

static bool has_server_feature_callback(const String &p_feature) {
	if (RenderingServer::get_singleton()) {
		if (RenderingServer::get_singleton()->has_os_feature(p_feature)) {
			return true;
		}
	}

	return false;
}

void preregister_server_types() {
	shader_types = memnew(ShaderTypes);

	GDREGISTER_CLASS(TextServerManager);
	GDREGISTER_VIRTUAL_CLASS(TextServer);
	GDREGISTER_CLASS(TextServerExtension);

	Engine::get_singleton()->add_singleton(Engine::Singleton("TextServerManager", TextServerManager::get_singleton(), "TextServerManager"));
}

void register_server_types() {
	OS::get_singleton()->set_has_server_feature_callback(has_server_feature_callback);

	GDREGISTER_VIRTUAL_CLASS(DisplayServer);
	GDREGISTER_VIRTUAL_CLASS(RenderingServer);
	GDREGISTER_CLASS(AudioServer);

	GDREGISTER_VIRTUAL_CLASS(PhysicsServer2D);
	GDREGISTER_VIRTUAL_CLASS(PhysicsServer3D);
	GDREGISTER_VIRTUAL_CLASS(NavigationServer2D);
	GDREGISTER_VIRTUAL_CLASS(NavigationServer3D);
	GDREGISTER_CLASS(XRServer);
	GDREGISTER_CLASS(CameraServer);

	GDREGISTER_VIRTUAL_CLASS(RenderingDevice);

	GDREGISTER_VIRTUAL_CLASS(XRInterface);
	GDREGISTER_CLASS(XRInterfaceExtension); // can't register this as virtual because we need a creation function for our extensions.
	GDREGISTER_CLASS(XRPose);
	GDREGISTER_CLASS(XRPositionalTracker);

	GDREGISTER_CLASS(AudioStream);
	GDREGISTER_CLASS(AudioStreamPlayback);
	GDREGISTER_VIRTUAL_CLASS(AudioStreamPlaybackResampled);
	GDREGISTER_CLASS(AudioStreamMicrophone);
	GDREGISTER_CLASS(AudioStreamRandomPitch);
	GDREGISTER_VIRTUAL_CLASS(AudioEffect);
	GDREGISTER_VIRTUAL_CLASS(AudioEffectInstance);
	GDREGISTER_CLASS(AudioEffectEQ);
	GDREGISTER_CLASS(AudioEffectFilter);
	GDREGISTER_CLASS(AudioBusLayout);

	GDREGISTER_CLASS(AudioStreamGenerator);
	GDREGISTER_VIRTUAL_CLASS(AudioStreamGeneratorPlayback);

	{
		//audio effects
		GDREGISTER_CLASS(AudioEffectAmplify);

		GDREGISTER_CLASS(AudioEffectReverb);

		GDREGISTER_CLASS(AudioEffectLowPassFilter);
		GDREGISTER_CLASS(AudioEffectHighPassFilter);
		GDREGISTER_CLASS(AudioEffectBandPassFilter);
		GDREGISTER_CLASS(AudioEffectNotchFilter);
		GDREGISTER_CLASS(AudioEffectBandLimitFilter);
		GDREGISTER_CLASS(AudioEffectLowShelfFilter);
		GDREGISTER_CLASS(AudioEffectHighShelfFilter);

		GDREGISTER_CLASS(AudioEffectEQ6);
		GDREGISTER_CLASS(AudioEffectEQ10);
		GDREGISTER_CLASS(AudioEffectEQ21);

		GDREGISTER_CLASS(AudioEffectDistortion);

		GDREGISTER_CLASS(AudioEffectStereoEnhance);

		GDREGISTER_CLASS(AudioEffectPanner);
		GDREGISTER_CLASS(AudioEffectChorus);
		GDREGISTER_CLASS(AudioEffectDelay);
		GDREGISTER_CLASS(AudioEffectCompressor);
		GDREGISTER_CLASS(AudioEffectLimiter);
		GDREGISTER_CLASS(AudioEffectPitchShift);
		GDREGISTER_CLASS(AudioEffectPhaser);

		GDREGISTER_CLASS(AudioEffectRecord);
		GDREGISTER_CLASS(AudioEffectSpectrumAnalyzer);
		GDREGISTER_VIRTUAL_CLASS(AudioEffectSpectrumAnalyzerInstance);

		GDREGISTER_CLASS(AudioEffectCapture);
	}

	GDREGISTER_VIRTUAL_CLASS(RenderingDevice);
	GDREGISTER_CLASS(RDTextureFormat);
	GDREGISTER_CLASS(RDTextureView);
	GDREGISTER_CLASS(RDAttachmentFormat);
	GDREGISTER_CLASS(RDFramebufferPass);
	GDREGISTER_CLASS(RDSamplerState);
	GDREGISTER_CLASS(RDVertexAttribute);
	GDREGISTER_CLASS(RDUniform);
	GDREGISTER_CLASS(RDPipelineRasterizationState);
	GDREGISTER_CLASS(RDPipelineMultisampleState);
	GDREGISTER_CLASS(RDPipelineDepthStencilState);
	GDREGISTER_CLASS(RDPipelineColorBlendStateAttachment);
	GDREGISTER_CLASS(RDPipelineColorBlendState);
	GDREGISTER_CLASS(RDShaderSource);
	GDREGISTER_CLASS(RDShaderSPIRV);
	GDREGISTER_CLASS(RDShaderFile);
	GDREGISTER_CLASS(RDPipelineSpecializationConstant);

	GDREGISTER_CLASS(CameraFeed);

	GDREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState2D);
	GDREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState2D);
	GDREGISTER_CLASS(PhysicsRayQueryParameters2D);
	GDREGISTER_CLASS(PhysicsPointQueryParameters2D);
	GDREGISTER_CLASS(PhysicsShapeQueryParameters2D);
	GDREGISTER_CLASS(PhysicsTestMotionParameters2D);
	GDREGISTER_CLASS(PhysicsTestMotionResult2D);

	GDREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState3D);
	GDREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState3D);
	GDREGISTER_CLASS(PhysicsRayQueryParameters3D);
	GDREGISTER_CLASS(PhysicsPointQueryParameters3D);
	GDREGISTER_CLASS(PhysicsShapeQueryParameters3D);
	GDREGISTER_CLASS(PhysicsTestMotionParameters3D);
	GDREGISTER_CLASS(PhysicsTestMotionResult3D);

	// Physics 2D
	GLOBAL_DEF(PhysicsServer2DManager::setting_property_name, "DEFAULT");
	ProjectSettings::get_singleton()->set_custom_property_info(PhysicsServer2DManager::setting_property_name, PropertyInfo(Variant::STRING, PhysicsServer2DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"));

	PhysicsServer2DManager::register_server("GodotPhysics2D", &_createGodotPhysics2DCallback);
	PhysicsServer2DManager::set_default_server("GodotPhysics2D");

	// Physics 3D
	GLOBAL_DEF(PhysicsServer3DManager::setting_property_name, "DEFAULT");
	ProjectSettings::get_singleton()->set_custom_property_info(PhysicsServer3DManager::setting_property_name, PropertyInfo(Variant::STRING, PhysicsServer3DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"));

	PhysicsServer3DManager::register_server("GodotPhysics3D", &_createGodotPhysics3DCallback);
	PhysicsServer3DManager::set_default_server("GodotPhysics3D");

	NativeExtensionManager::get_singleton()->initialize_extensions(NativeExtension::INITIALIZATION_LEVEL_SERVERS);
}

void unregister_server_types() {
	NativeExtensionManager::get_singleton()->deinitialize_extensions(NativeExtension::INITIALIZATION_LEVEL_SERVERS);

	memdelete(shader_types);
}

void register_server_singletons() {
	Engine::get_singleton()->add_singleton(Engine::Singleton("DisplayServer", DisplayServer::get_singleton(), "DisplayServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("RenderingServer", RenderingServer::get_singleton(), "RenderingServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("AudioServer", AudioServer::get_singleton(), "AudioServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer2D", PhysicsServer2D::get_singleton(), "PhysicsServer2D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer3D", PhysicsServer3D::get_singleton(), "PhysicsServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer2D", NavigationServer2D::get_singleton_mut(), "NavigationServer2D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer3D", NavigationServer3D::get_singleton_mut(), "NavigationServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("XRServer", XRServer::get_singleton(), "XRServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("CameraServer", CameraServer::get_singleton(), "CameraServer"));
}
