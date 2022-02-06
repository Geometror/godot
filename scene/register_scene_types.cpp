/*************************************************************************/
/*  register_scene_types.cpp                                             */
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

#include "register_scene_types.h"

#include "core/config/project_settings.h"
#include "core/extension/native_extension_manager.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "scene/debugger/scene_debugger.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/dialogs.h"
#include "scene/main/scene_tree.h"
#include "scene/multiplayer/scene_cache_interface.h"
#include "scene/multiplayer/scene_replication_interface.h"
#include "scene/multiplayer/scene_rpc_interface.h"
#include "scene/resources/default_theme/default_theme.h"
#include "scene/resources/font.h"
#include "scene/resources/material.h"
#include "scene/resources/particles_material.h"
#include "scene/resources/resource_format_text.h"
#include "scene/resources/skeleton_modification_3d.h"
#include "scene/resources/skeleton_modification_3d_ccdik.h"
#include "scene/resources/skeleton_modification_3d_fabrik.h"
#include "scene/resources/skeleton_modification_3d_jiggle.h"
#include "scene/resources/skeleton_modification_3d_lookat.h"
#include "scene/resources/skeleton_modification_3d_stackholder.h"
#include "scene/resources/skeleton_modification_3d_twoboneik.h"
#include "scene/resources/skeleton_modification_stack_3d.h"
#include "scene/resources/sky_material.h"
#include "scene/resources/texture.h"
#include "scene/scene_string_names.h"
#include "core/error/error_macros.h"
#include "core/extension/native_extension.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"
#include "scene/main/node.h"
#include "scene/resources/canvas_item_material.h"
#include "scene/resources/shader.h"
#include "scene/resources/theme.h"
#include "servers/display_server.h"
#include "servers/rendering_server.h"

class AnimatableBody2D;
class AnimatableBody3D;
class AnimatedSprite2D;
class AnimatedSprite3D;
class Animation;
class AnimationNode;
class AnimationNodeAdd2;
class AnimationNodeAdd3;
class AnimationNodeAnimation;
class AnimationNodeBlend2;
class AnimationNodeBlend3;
class AnimationNodeBlendSpace1D;
class AnimationNodeBlendSpace2D;
class AnimationNodeBlendTree;
class AnimationNodeOneShot;
class AnimationNodeOutput;
class AnimationNodeStateMachine;
class AnimationNodeStateMachinePlayback;
class AnimationNodeStateMachineTransition;
class AnimationNodeTimeScale;
class AnimationNodeTimeSeek;
class AnimationNodeTransition;
class AnimationPlayer;
class AnimationRootNode;
class AnimationTree;
class Area2D;
class Area3D;
class ArrayMesh;
class AspectRatioContainer;
class AudioListener2D;
class AudioListener3D;
class AudioStreamPlayer2D;
class AudioStreamPlayer3D;
class AudioStreamPlayer;
class AudioStreamSample;
class BackBufferCopy;
class BaseButton;
class BitMap;
class Bone2D;
class BoneAttachment3D;
class BoxContainer;
class BoxMesh;
class BoxShape3D;
class Button;
class ButtonGroup;
class CPUParticles2D;
class CPUParticles3D;
class CallbackTweener;
class Camera2D;
class Camera3D;
class CameraEffects;
class CanvasGroup;
class CanvasItem;
class CanvasLayer;
class CanvasModulate;
class CanvasTexture;
class CapsuleMesh;
class CapsuleShape2D;
class CapsuleShape3D;
class CenterContainer;
class CharFXTransform;
class CharacterBody2D;
class CharacterBody3D;
class CheckBox;
class CheckButton;
class CircleShape2D;
class CodeEdit;
class CodeHighlighter;
class CollisionObject2D;
class CollisionObject3D;
class CollisionPolygon2D;
class CollisionPolygon3D;
class CollisionShape2D;
class CollisionShape3D;
class ColorRect;
class ConcavePolygonShape2D;
class ConcavePolygonShape3D;
class ConeTwistJoint3D;
class Container;
class Control;
class ConvexPolygonShape2D;
class ConvexPolygonShape3D;
class Curve2D;
class Curve3D;
class Curve;
class CylinderMesh;
class CylinderShape3D;
class DampedSpringJoint2D;
class Decal;
class DirectionalLight2D;
class DirectionalLight3D;
class Environment;
class FileDialog;
class FlowContainer;
class FogMaterial;
class FogVolume;
class GPUParticles2D;
class GPUParticles3D;
class GPUParticlesAttractor3D;
class GPUParticlesAttractorBox3D;
class GPUParticlesAttractorSphere3D;
class GPUParticlesAttractorVectorField3D;
class GPUParticlesCollision3D;
class GPUParticlesCollisionBox3D;
class GPUParticlesCollisionHeightField3D;
class GPUParticlesCollisionSDF3D;
class GPUParticlesCollisionSphere3D;
class Generic6DOFJoint3D;
class GeometryInstance3D;
class Gradient;
class GraphEdit;
class GraphNode;
class GridContainer;
class GrooveJoint2D;
class HBoxContainer;
class HFlowContainer;
class HScrollBar;
class HSeparator;
class HSlider;
class HSplitContainer;
class HTTPRequest;
class HeightMapShape3D;
class HingeJoint3D;
class ImmediateMesh;
class ImporterMesh;
class ImporterMeshInstance3D;
class InstancePlaceholder;
class IntervalTweener;
class ItemList;
class Joint2D;
class Joint3D;
class KinematicCollision2D;
class KinematicCollision3D;
class Label;
class Light2D;
class Light3D;
class LightOccluder2D;
class LightmapGI;
class LightmapGIData;
class LightmapProbe;
class Lightmapper;
class Line2D;
class LineEdit;
class LinkButton;
class MarginContainer;
class MenuButton;
class Mesh;
class MeshDataTool;
class MeshInstance2D;
class MeshInstance3D;
class MeshLibrary;
class MethodTweener;
class MultiMesh;
class MultiMeshInstance2D;
class MultiMeshInstance3D;
class MultiplayerSpawner;
class MultiplayerSynchronizer;
class NavigationAgent2D;
class NavigationAgent3D;
class NavigationMesh;
class NavigationObstacle2D;
class NavigationObstacle3D;
class NavigationPolygon;
class NavigationRegion2D;
class NavigationRegion3D;
class NinePatchRect;
class Node2D;
class Node3D;
class Node3DGizmo;
class Occluder3D;
class OccluderInstance3D;
class OccluderPolygon2D;
class OmniLight3D;
class OptionButton;
class PackedScene;
class Panel;
class PanelContainer;
class ParallaxBackground;
class ParallaxLayer;
class Path2D;
class Path3D;
class PathFollow2D;
class PathFollow3D;
class PhysicalBone2D;
class PhysicalBone3D;
class PhysicsBody2D;
class PhysicsBody3D;
class PhysicsMaterial;
class PinJoint2D;
class PinJoint3D;
class PlaneMesh;
class PointLight2D;
class PointMesh;
class Polygon2D;
class PolygonPathFinder;
class Popup;
class PopupMenu;
class PopupPanel;
class Position2D;
class Position3D;
class PrimitiveMesh;
class PrismMesh;
class ProgressBar;
class PropertyTweener;
class QuadMesh;
class Range;
class RayCast2D;
class RayCast3D;
class RectangleShape2D;
class ReferenceRect;
class ReflectionProbe;
class RemoteTransform2D;
class RemoteTransform3D;
class ResourcePreloader;
class RibbonTrailMesh;
class RichTextEffect;
class RichTextLabel;
class RigidDynamicBody2D;
class RigidDynamicBody3D;
class RootMotionView;
class SceneReplicationConfig;
class SceneState;
class ScrollBar;
class ScrollContainer;
class SegmentShape2D;
class SeparationRayShape2D;
class SeparationRayShape3D;
class Separator;
class ShaderGlobalsOverride;
class Shape2D;
class Shape3D;
class ShapeCast2D;
class Skeleton2D;
class Skeleton3D;
class SkeletonIK3D;
class SkeletonModification2D;
class SkeletonModification2DCCDIK;
class SkeletonModification2DFABRIK;
class SkeletonModification2DJiggle;
class SkeletonModification2DLookAt;
class SkeletonModification2DPhysicalBones;
class SkeletonModification2DStackHolder;
class SkeletonModification2DTwoBoneIK;
class SkeletonModificationStack2D;
class Skin;
class SkinReference;
class Sky;
class Slider;
class SliderJoint3D;
class SoftDynamicBody3D;
class SphereMesh;
class SphereShape3D;
class SpinBox;
class SplitContainer;
class SpotLight3D;
class SpringArm3D;
class Sprite2D;
class Sprite3D;
class SpriteBase3D;
class SpriteFrames;
class StaticBody2D;
class StaticBody3D;
class StyleBox;
class StyleBoxEmpty;
class StyleBoxFlat;
class StyleBoxLine;
class StyleBoxTexture;
class SubViewport;
class SubViewportContainer;
class SurfaceTool;
class SyntaxHighlighter;
class TabBar;
class TabContainer;
class TextEdit;
class TextLine;
class TextParagraph;
class TextureButton;
class TextureProgressBar;
class TextureRect;
class TileData;
class TileMap;
class TileMapPattern;
class TileSet;
class TileSetAtlasSource;
class TileSetScenesCollectionSource;
class TileSetSource;
class Timer;
class TouchScreenButton;
class Tree;
class TreeItem;
class TubeTrailMesh;
class Tween;
class Tweener;
class VBoxContainer;
class VFlowContainer;
class VScrollBar;
class VSeparator;
class VSlider;
class VSplitContainer;
class VehicleBody3D;
class VehicleWheel3D;
class VelocityTracker3D;
class VideoStream;
class VideoStreamPlayer;
class Viewport;
class ViewportTexture;
class VisibleOnScreenEnabler2D;
class VisibleOnScreenEnabler3D;
class VisibleOnScreenNotifier2D;
class VisibleOnScreenNotifier3D;
class VisualInstance3D;
class VisualShader;
class VisualShaderNode;
class VisualShaderNodeBillboard;
class VisualShaderNodeBooleanConstant;
class VisualShaderNodeBooleanUniform;
class VisualShaderNodeClamp;
class VisualShaderNodeColorConstant;
class VisualShaderNodeColorFunc;
class VisualShaderNodeColorOp;
class VisualShaderNodeColorUniform;
class VisualShaderNodeComment;
class VisualShaderNodeCompare;
class VisualShaderNodeConstant;
class VisualShaderNodeCubemap;
class VisualShaderNodeCubemapUniform;
class VisualShaderNodeCurveTexture;
class VisualShaderNodeCurveXYZTexture;
class VisualShaderNodeCustom;
class VisualShaderNodeDerivativeFunc;
class VisualShaderNodeDeterminant;
class VisualShaderNodeDotProduct;
class VisualShaderNodeExpression;
class VisualShaderNodeFaceForward;
class VisualShaderNodeFloatConstant;
class VisualShaderNodeFloatFunc;
class VisualShaderNodeFloatOp;
class VisualShaderNodeFloatUniform;
class VisualShaderNodeFresnel;
class VisualShaderNodeGlobalExpression;
class VisualShaderNodeGroupBase;
class VisualShaderNodeIf;
class VisualShaderNodeInput;
class VisualShaderNodeIntConstant;
class VisualShaderNodeIntFunc;
class VisualShaderNodeIntOp;
class VisualShaderNodeIntUniform;
class VisualShaderNodeIs;
class VisualShaderNodeMix;
class VisualShaderNodeMultiplyAdd;
class VisualShaderNodeOuterProduct;
class VisualShaderNodeOutput;
class VisualShaderNodeParticleAccelerator;
class VisualShaderNodeParticleBoxEmitter;
class VisualShaderNodeParticleConeVelocity;
class VisualShaderNodeParticleEmit;
class VisualShaderNodeParticleEmitter;
class VisualShaderNodeParticleMeshEmitter;
class VisualShaderNodeParticleMultiplyByAxisAngle;
class VisualShaderNodeParticleOutput;
class VisualShaderNodeParticleRandomness;
class VisualShaderNodeParticleRingEmitter;
class VisualShaderNodeParticleSphereEmitter;
class VisualShaderNodeResizableBase;
class VisualShaderNodeSDFRaymarch;
class VisualShaderNodeSDFToScreenUV;
class VisualShaderNodeSample3D;
class VisualShaderNodeScreenUVToSDF;
class VisualShaderNodeSmoothStep;
class VisualShaderNodeStep;
class VisualShaderNodeSwitch;
class VisualShaderNodeTexture2DArray;
class VisualShaderNodeTexture2DArrayUniform;
class VisualShaderNodeTexture3D;
class VisualShaderNodeTexture3DUniform;
class VisualShaderNodeTexture;
class VisualShaderNodeTextureSDF;
class VisualShaderNodeTextureSDFNormal;
class VisualShaderNodeTextureUniform;
class VisualShaderNodeTextureUniformTriplanar;
class VisualShaderNodeTransformCompose;
class VisualShaderNodeTransformConstant;
class VisualShaderNodeTransformDecompose;
class VisualShaderNodeTransformFunc;
class VisualShaderNodeTransformOp;
class VisualShaderNodeTransformUniform;
class VisualShaderNodeTransformVecMult;
class VisualShaderNodeUVFunc;
class VisualShaderNodeUniform;
class VisualShaderNodeUniformRef;
class VisualShaderNodeVec2Constant;
class VisualShaderNodeVec2Uniform;
class VisualShaderNodeVec3Constant;
class VisualShaderNodeVec3Uniform;
class VisualShaderNodeVectorBase;
class VisualShaderNodeVectorCompose;
class VisualShaderNodeVectorDecompose;
class VisualShaderNodeVectorDistance;
class VisualShaderNodeVectorFunc;
class VisualShaderNodeVectorLen;
class VisualShaderNodeVectorOp;
class VisualShaderNodeVectorRefract;
class VoxelGI;
class VoxelGIData;
class Window;
class World2D;
class World3D;
class WorldBoundaryShape2D;
class WorldBoundaryShape3D;
class WorldEnvironment;
class XRAnchor3D;
class XRCamera3D;
class XRController3D;
class XRNode3D;
class XROrigin3D;

static Ref<ResourceFormatSaverText> resource_saver_text;
static Ref<ResourceFormatLoaderText> resource_loader_text;

static Ref<ResourceFormatLoaderStreamTexture2D> resource_loader_stream_texture;
static Ref<ResourceFormatLoaderStreamTextureLayered> resource_loader_texture_layered;
static Ref<ResourceFormatLoaderStreamTexture3D> resource_loader_texture_3d;

static Ref<ResourceFormatSaverShader> resource_saver_shader;
static Ref<ResourceFormatLoaderShader> resource_loader_shader;

void register_scene_types() {
	SceneStringNames::create();

	OS::get_singleton()->yield(); // may take time to init

	Node::init_node_hrcr();

	resource_loader_stream_texture.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_stream_texture);

	resource_loader_texture_layered.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_layered);

	resource_loader_texture_3d.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_3d);

	resource_saver_text.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_text, true);

	resource_loader_text.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_text, true);

	resource_saver_shader.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_shader, true);

	resource_loader_shader.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_shader, true);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(Object);

	GDREGISTER_CLASS(Node);
	GDREGISTER_VIRTUAL_CLASS(InstancePlaceholder);

	GDREGISTER_VIRTUAL_CLASS(Viewport);
	GDREGISTER_CLASS(SubViewport);
	GDREGISTER_CLASS(ViewportTexture);
	GDREGISTER_CLASS(HTTPRequest);
	GDREGISTER_CLASS(MultiplayerSpawner);
	GDREGISTER_CLASS(MultiplayerSynchronizer);
	GDREGISTER_CLASS(Timer);
	GDREGISTER_CLASS(CanvasLayer);
	GDREGISTER_CLASS(CanvasModulate);
	GDREGISTER_CLASS(ResourcePreloader);
	GDREGISTER_CLASS(Window);

	/* REGISTER GUI */

	GDREGISTER_CLASS(ButtonGroup);
	GDREGISTER_VIRTUAL_CLASS(BaseButton);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(Control);
	GDREGISTER_CLASS(Button);
	GDREGISTER_CLASS(Label);
	GDREGISTER_VIRTUAL_CLASS(ScrollBar);
	GDREGISTER_CLASS(HScrollBar);
	GDREGISTER_CLASS(VScrollBar);
	GDREGISTER_CLASS(ProgressBar);
	GDREGISTER_VIRTUAL_CLASS(Slider);
	GDREGISTER_CLASS(HSlider);
	GDREGISTER_CLASS(VSlider);
	GDREGISTER_CLASS(Popup);
	GDREGISTER_CLASS(PopupPanel);
	GDREGISTER_CLASS(MenuButton);
	GDREGISTER_CLASS(CheckBox);
	GDREGISTER_CLASS(CheckButton);
	GDREGISTER_CLASS(LinkButton);
	GDREGISTER_CLASS(Panel);
	GDREGISTER_VIRTUAL_CLASS(Range);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(TextureRect);
	GDREGISTER_CLASS(ColorRect);
	GDREGISTER_CLASS(NinePatchRect);
	GDREGISTER_CLASS(ReferenceRect);
	GDREGISTER_CLASS(AspectRatioContainer);
	GDREGISTER_CLASS(TabContainer);
	GDREGISTER_CLASS(TabBar);
	GDREGISTER_VIRTUAL_CLASS(Separator);
	GDREGISTER_CLASS(HSeparator);
	GDREGISTER_CLASS(VSeparator);
	GDREGISTER_CLASS(TextureButton);
	GDREGISTER_CLASS(Container);
	GDREGISTER_VIRTUAL_CLASS(BoxContainer);
	GDREGISTER_CLASS(HBoxContainer);
	GDREGISTER_CLASS(VBoxContainer);
	GDREGISTER_CLASS(GridContainer);
	GDREGISTER_CLASS(CenterContainer);
	GDREGISTER_CLASS(ScrollContainer);
	GDREGISTER_CLASS(PanelContainer);
	GDREGISTER_VIRTUAL_CLASS(FlowContainer);
	GDREGISTER_CLASS(HFlowContainer);
	GDREGISTER_CLASS(VFlowContainer);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(TextureProgressBar);
	GDREGISTER_CLASS(ItemList);

	GDREGISTER_CLASS(LineEdit);
	GDREGISTER_CLASS(VideoStreamPlayer);

#ifndef ADVANCED_GUI_DISABLED
	GDREGISTER_CLASS(FileDialog);

	GDREGISTER_CLASS(PopupMenu);
	GDREGISTER_CLASS(Tree);

	GDREGISTER_CLASS(TextEdit);
	GDREGISTER_CLASS(CodeEdit);
	GDREGISTER_CLASS(SyntaxHighlighter);
	GDREGISTER_CLASS(CodeHighlighter);

	GDREGISTER_VIRTUAL_CLASS(TreeItem);
	GDREGISTER_CLASS(OptionButton);
	GDREGISTER_CLASS(SpinBox);
	GDREGISTER_CLASS(ColorPicker);
	GDREGISTER_CLASS(ColorPickerButton);
	GDREGISTER_CLASS(RichTextLabel);
	GDREGISTER_CLASS(RichTextEffect);
	GDREGISTER_CLASS(CharFXTransform);

	GDREGISTER_CLASS(AcceptDialog);
	GDREGISTER_CLASS(ConfirmationDialog);

	GDREGISTER_CLASS(MarginContainer);
	GDREGISTER_CLASS(SubViewportContainer);
	GDREGISTER_VIRTUAL_CLASS(SplitContainer);
	GDREGISTER_CLASS(HSplitContainer);
	GDREGISTER_CLASS(VSplitContainer);

	GDREGISTER_CLASS(GraphNode);
	GDREGISTER_CLASS(GraphEdit);

	OS::get_singleton()->yield(); // may take time to init

	bool swap_cancel_ok = false;
	if (DisplayServer::get_singleton()) {
		swap_cancel_ok = GLOBAL_DEF_NOVAL("gui/common/swap_cancel_ok", bool(DisplayServer::get_singleton()->get_swap_cancel_ok()));
	}
	AcceptDialog::set_swap_cancel_ok(swap_cancel_ok);
#endif

	/* REGISTER ANIMATION */

	GDREGISTER_CLASS(AnimationPlayer);
	GDREGISTER_CLASS(Tween);
	GDREGISTER_VIRTUAL_CLASS(Tweener);
	GDREGISTER_CLASS(PropertyTweener);
	GDREGISTER_CLASS(IntervalTweener);
	GDREGISTER_CLASS(CallbackTweener);
	GDREGISTER_CLASS(MethodTweener);

	GDREGISTER_CLASS(AnimationTree);
	GDREGISTER_CLASS(AnimationNode);
	GDREGISTER_CLASS(AnimationRootNode);
	GDREGISTER_CLASS(AnimationNodeBlendTree);
	GDREGISTER_CLASS(AnimationNodeBlendSpace1D);
	GDREGISTER_CLASS(AnimationNodeBlendSpace2D);
	GDREGISTER_CLASS(AnimationNodeStateMachine);
	GDREGISTER_CLASS(AnimationNodeStateMachinePlayback);

	GDREGISTER_CLASS(AnimationNodeStateMachineTransition);
	GDREGISTER_CLASS(AnimationNodeOutput);
	GDREGISTER_CLASS(AnimationNodeOneShot);
	GDREGISTER_CLASS(AnimationNodeAnimation);
	GDREGISTER_CLASS(AnimationNodeAdd2);
	GDREGISTER_CLASS(AnimationNodeAdd3);
	GDREGISTER_CLASS(AnimationNodeBlend2);
	GDREGISTER_CLASS(AnimationNodeBlend3);
	GDREGISTER_CLASS(AnimationNodeTimeScale);
	GDREGISTER_CLASS(AnimationNodeTimeSeek);
	GDREGISTER_CLASS(AnimationNodeTransition);

	GDREGISTER_CLASS(ShaderGlobalsOverride); // can be used in any shader

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER 3D */

#ifndef _3D_DISABLED
	GDREGISTER_CLASS(Node3D);
	GDREGISTER_VIRTUAL_CLASS(Node3DGizmo);
	GDREGISTER_CLASS(Skin);
	GDREGISTER_VIRTUAL_CLASS(SkinReference);
	GDREGISTER_CLASS(Skeleton3D);
	GDREGISTER_CLASS(ImporterMesh);
	GDREGISTER_CLASS(ImporterMeshInstance3D);
	GDREGISTER_VIRTUAL_CLASS(VisualInstance3D);
	GDREGISTER_VIRTUAL_CLASS(GeometryInstance3D);
	GDREGISTER_CLASS(Camera3D);
	GDREGISTER_CLASS(AudioListener3D);
	GDREGISTER_CLASS(XRCamera3D);
	GDREGISTER_VIRTUAL_CLASS(XRNode3D);
	GDREGISTER_CLASS(XRController3D);
	GDREGISTER_CLASS(XRAnchor3D);
	GDREGISTER_CLASS(XROrigin3D);
	GDREGISTER_CLASS(MeshInstance3D);
	GDREGISTER_CLASS(OccluderInstance3D);
	GDREGISTER_CLASS(Occluder3D);
	GDREGISTER_VIRTUAL_CLASS(SpriteBase3D);
	GDREGISTER_CLASS(Sprite3D);
	GDREGISTER_CLASS(AnimatedSprite3D);
	GDREGISTER_VIRTUAL_CLASS(Light3D);
	GDREGISTER_CLASS(DirectionalLight3D);
	GDREGISTER_CLASS(OmniLight3D);
	GDREGISTER_CLASS(SpotLight3D);
	GDREGISTER_CLASS(ReflectionProbe);
	GDREGISTER_CLASS(Decal);
	GDREGISTER_CLASS(VoxelGI);
	GDREGISTER_CLASS(VoxelGIData);
	GDREGISTER_CLASS(LightmapGI);
	GDREGISTER_CLASS(LightmapGIData);
	GDREGISTER_CLASS(LightmapProbe);
	GDREGISTER_VIRTUAL_CLASS(Lightmapper);
	GDREGISTER_CLASS(GPUParticles3D);
	GDREGISTER_VIRTUAL_CLASS(GPUParticlesCollision3D);
	GDREGISTER_CLASS(GPUParticlesCollisionBox3D);
	GDREGISTER_CLASS(GPUParticlesCollisionSphere3D);
	GDREGISTER_CLASS(GPUParticlesCollisionSDF3D);
	GDREGISTER_CLASS(GPUParticlesCollisionHeightField3D);
	GDREGISTER_VIRTUAL_CLASS(GPUParticlesAttractor3D);
	GDREGISTER_CLASS(GPUParticlesAttractorBox3D);
	GDREGISTER_CLASS(GPUParticlesAttractorSphere3D);
	GDREGISTER_CLASS(GPUParticlesAttractorVectorField3D);
	GDREGISTER_CLASS(CPUParticles3D);
	GDREGISTER_CLASS(Position3D);

	GDREGISTER_CLASS(RootMotionView);
	ClassDB::set_class_enabled("RootMotionView", false); // disabled by default, enabled by editor

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_VIRTUAL_CLASS(CollisionObject3D);
	GDREGISTER_VIRTUAL_CLASS(PhysicsBody3D);
	GDREGISTER_CLASS(StaticBody3D);
	GDREGISTER_CLASS(AnimatableBody3D);
	GDREGISTER_CLASS(RigidDynamicBody3D);
	GDREGISTER_CLASS(KinematicCollision3D);
	GDREGISTER_CLASS(CharacterBody3D);
	GDREGISTER_CLASS(SpringArm3D);

	GDREGISTER_CLASS(PhysicalBone3D);
	GDREGISTER_CLASS(SoftDynamicBody3D);

	GDREGISTER_CLASS(SkeletonIK3D);
	GDREGISTER_CLASS(BoneAttachment3D);

	GDREGISTER_CLASS(VehicleBody3D);
	GDREGISTER_CLASS(VehicleWheel3D);
	GDREGISTER_CLASS(Area3D);
	GDREGISTER_CLASS(CollisionShape3D);
	GDREGISTER_CLASS(CollisionPolygon3D);
	GDREGISTER_CLASS(RayCast3D);
	GDREGISTER_CLASS(MultiMeshInstance3D);

	GDREGISTER_CLASS(Curve3D);
	GDREGISTER_CLASS(Path3D);
	GDREGISTER_CLASS(PathFollow3D);
	GDREGISTER_CLASS(VisibleOnScreenNotifier3D);
	GDREGISTER_CLASS(VisibleOnScreenEnabler3D);
	GDREGISTER_CLASS(WorldEnvironment);
	GDREGISTER_CLASS(FogVolume);
	GDREGISTER_CLASS(FogMaterial);
	GDREGISTER_CLASS(RemoteTransform3D);

	GDREGISTER_VIRTUAL_CLASS(Joint3D);
	GDREGISTER_CLASS(PinJoint3D);
	GDREGISTER_CLASS(HingeJoint3D);
	GDREGISTER_CLASS(SliderJoint3D);
	GDREGISTER_CLASS(ConeTwistJoint3D);
	GDREGISTER_CLASS(Generic6DOFJoint3D);

	GDREGISTER_CLASS(NavigationRegion3D);
	GDREGISTER_CLASS(NavigationAgent3D);
	GDREGISTER_CLASS(NavigationObstacle3D);

	OS::get_singleton()->yield(); // may take time to init
#endif

	/* REGISTER SHADER */

	GDREGISTER_CLASS(Shader);
	GDREGISTER_CLASS(VisualShader);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNode);
	GDREGISTER_CLASS(VisualShaderNodeCustom);
	GDREGISTER_CLASS(VisualShaderNodeInput);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeOutput);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeResizableBase);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeGroupBase);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeConstant);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeVectorBase);
	GDREGISTER_CLASS(VisualShaderNodeComment);
	GDREGISTER_CLASS(VisualShaderNodeFloatConstant);
	GDREGISTER_CLASS(VisualShaderNodeIntConstant);
	GDREGISTER_CLASS(VisualShaderNodeBooleanConstant);
	GDREGISTER_CLASS(VisualShaderNodeColorConstant);
	GDREGISTER_CLASS(VisualShaderNodeVec2Constant);
	GDREGISTER_CLASS(VisualShaderNodeVec3Constant);
	GDREGISTER_CLASS(VisualShaderNodeTransformConstant);
	GDREGISTER_CLASS(VisualShaderNodeFloatOp);
	GDREGISTER_CLASS(VisualShaderNodeIntOp);
	GDREGISTER_CLASS(VisualShaderNodeVectorOp);
	GDREGISTER_CLASS(VisualShaderNodeColorOp);
	GDREGISTER_CLASS(VisualShaderNodeTransformOp);
	GDREGISTER_CLASS(VisualShaderNodeTransformVecMult);
	GDREGISTER_CLASS(VisualShaderNodeFloatFunc);
	GDREGISTER_CLASS(VisualShaderNodeIntFunc);
	GDREGISTER_CLASS(VisualShaderNodeVectorFunc);
	GDREGISTER_CLASS(VisualShaderNodeColorFunc);
	GDREGISTER_CLASS(VisualShaderNodeTransformFunc);
	GDREGISTER_CLASS(VisualShaderNodeUVFunc);
	GDREGISTER_CLASS(VisualShaderNodeDotProduct);
	GDREGISTER_CLASS(VisualShaderNodeVectorLen);
	GDREGISTER_CLASS(VisualShaderNodeDeterminant);
	GDREGISTER_CLASS(VisualShaderNodeDerivativeFunc);
	GDREGISTER_CLASS(VisualShaderNodeClamp);
	GDREGISTER_CLASS(VisualShaderNodeFaceForward);
	GDREGISTER_CLASS(VisualShaderNodeOuterProduct);
	GDREGISTER_CLASS(VisualShaderNodeSmoothStep);
	GDREGISTER_CLASS(VisualShaderNodeStep);
	GDREGISTER_CLASS(VisualShaderNodeVectorDistance);
	GDREGISTER_CLASS(VisualShaderNodeVectorRefract);
	GDREGISTER_CLASS(VisualShaderNodeMix);
	GDREGISTER_CLASS(VisualShaderNodeVectorCompose);
	GDREGISTER_CLASS(VisualShaderNodeTransformCompose);
	GDREGISTER_CLASS(VisualShaderNodeVectorDecompose);
	GDREGISTER_CLASS(VisualShaderNodeTransformDecompose);
	GDREGISTER_CLASS(VisualShaderNodeTexture);
	GDREGISTER_CLASS(VisualShaderNodeCurveTexture);
	GDREGISTER_CLASS(VisualShaderNodeCurveXYZTexture);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeSample3D);
	GDREGISTER_CLASS(VisualShaderNodeTexture2DArray);
	GDREGISTER_CLASS(VisualShaderNodeTexture3D);
	GDREGISTER_CLASS(VisualShaderNodeCubemap);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeUniform);
	GDREGISTER_CLASS(VisualShaderNodeUniformRef);
	GDREGISTER_CLASS(VisualShaderNodeFloatUniform);
	GDREGISTER_CLASS(VisualShaderNodeIntUniform);
	GDREGISTER_CLASS(VisualShaderNodeBooleanUniform);
	GDREGISTER_CLASS(VisualShaderNodeColorUniform);
	GDREGISTER_CLASS(VisualShaderNodeVec2Uniform);
	GDREGISTER_CLASS(VisualShaderNodeVec3Uniform);
	GDREGISTER_CLASS(VisualShaderNodeTransformUniform);
	GDREGISTER_CLASS(VisualShaderNodeTextureUniform);
	GDREGISTER_CLASS(VisualShaderNodeTextureUniformTriplanar);
	GDREGISTER_CLASS(VisualShaderNodeTexture2DArrayUniform);
	GDREGISTER_CLASS(VisualShaderNodeTexture3DUniform);
	GDREGISTER_CLASS(VisualShaderNodeCubemapUniform);
	GDREGISTER_CLASS(VisualShaderNodeIf);
	GDREGISTER_CLASS(VisualShaderNodeSwitch);
	GDREGISTER_CLASS(VisualShaderNodeFresnel);
	GDREGISTER_CLASS(VisualShaderNodeExpression);
	GDREGISTER_CLASS(VisualShaderNodeGlobalExpression);
	GDREGISTER_CLASS(VisualShaderNodeIs);
	GDREGISTER_CLASS(VisualShaderNodeCompare);
	GDREGISTER_CLASS(VisualShaderNodeMultiplyAdd);
	GDREGISTER_CLASS(VisualShaderNodeBillboard);

	GDREGISTER_CLASS(VisualShaderNodeSDFToScreenUV);
	GDREGISTER_CLASS(VisualShaderNodeScreenUVToSDF);
	GDREGISTER_CLASS(VisualShaderNodeTextureSDF);
	GDREGISTER_CLASS(VisualShaderNodeTextureSDFNormal);
	GDREGISTER_CLASS(VisualShaderNodeSDFRaymarch);

	GDREGISTER_CLASS(VisualShaderNodeParticleOutput);
	GDREGISTER_VIRTUAL_CLASS(VisualShaderNodeParticleEmitter);
	GDREGISTER_CLASS(VisualShaderNodeParticleSphereEmitter);
	GDREGISTER_CLASS(VisualShaderNodeParticleBoxEmitter);
	GDREGISTER_CLASS(VisualShaderNodeParticleRingEmitter);
	GDREGISTER_CLASS(VisualShaderNodeParticleMeshEmitter);
	GDREGISTER_CLASS(VisualShaderNodeParticleMultiplyByAxisAngle);
	GDREGISTER_CLASS(VisualShaderNodeParticleConeVelocity);
	GDREGISTER_CLASS(VisualShaderNodeParticleRandomness);
	GDREGISTER_CLASS(VisualShaderNodeParticleAccelerator);
	GDREGISTER_CLASS(VisualShaderNodeParticleEmit);

	GDREGISTER_CLASS(ShaderMaterial);
	GDREGISTER_VIRTUAL_CLASS(CanvasItem);
	GDREGISTER_CLASS(CanvasTexture);
	GDREGISTER_CLASS(CanvasItemMaterial);
	SceneTree::add_idle_callback(CanvasItemMaterial::flush_changes);
	CanvasItemMaterial::init_shaders();

	/* REGISTER 2D */

	GDREGISTER_CLASS(Node2D);
	GDREGISTER_CLASS(CanvasGroup);
	GDREGISTER_CLASS(CPUParticles2D);
	GDREGISTER_CLASS(GPUParticles2D);
	GDREGISTER_CLASS(Sprite2D);
	GDREGISTER_CLASS(SpriteFrames);
	GDREGISTER_CLASS(AnimatedSprite2D);
	GDREGISTER_CLASS(Position2D);
	GDREGISTER_CLASS(Line2D);
	GDREGISTER_CLASS(MeshInstance2D);
	GDREGISTER_CLASS(MultiMeshInstance2D);
	GDREGISTER_VIRTUAL_CLASS(CollisionObject2D);
	GDREGISTER_VIRTUAL_CLASS(PhysicsBody2D);
	GDREGISTER_CLASS(StaticBody2D);
	GDREGISTER_CLASS(AnimatableBody2D);
	GDREGISTER_CLASS(RigidDynamicBody2D);
	GDREGISTER_CLASS(CharacterBody2D);
	GDREGISTER_CLASS(KinematicCollision2D);
	GDREGISTER_CLASS(Area2D);
	GDREGISTER_CLASS(CollisionShape2D);
	GDREGISTER_CLASS(CollisionPolygon2D);
	GDREGISTER_CLASS(RayCast2D);
	GDREGISTER_CLASS(ShapeCast2D);
	GDREGISTER_CLASS(VisibleOnScreenNotifier2D);
	GDREGISTER_CLASS(VisibleOnScreenEnabler2D);
	GDREGISTER_CLASS(Polygon2D);
	GDREGISTER_CLASS(Skeleton2D);
	GDREGISTER_CLASS(Bone2D);
	GDREGISTER_VIRTUAL_CLASS(Light2D);
	GDREGISTER_CLASS(PointLight2D);
	GDREGISTER_CLASS(DirectionalLight2D);
	GDREGISTER_CLASS(LightOccluder2D);
	GDREGISTER_CLASS(OccluderPolygon2D);
	GDREGISTER_CLASS(BackBufferCopy);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(Camera2D);
	GDREGISTER_CLASS(AudioListener2D);
	GDREGISTER_VIRTUAL_CLASS(Joint2D);
	GDREGISTER_CLASS(PinJoint2D);
	GDREGISTER_CLASS(GrooveJoint2D);
	GDREGISTER_CLASS(DampedSpringJoint2D);
	GDREGISTER_CLASS(TileSet);
	GDREGISTER_VIRTUAL_CLASS(TileSetSource);
	GDREGISTER_CLASS(TileSetAtlasSource);
	GDREGISTER_CLASS(TileSetScenesCollectionSource);
	GDREGISTER_CLASS(TileMapPattern);
	GDREGISTER_CLASS(TileData);
	GDREGISTER_CLASS(TileMap);
	GDREGISTER_CLASS(ParallaxBackground);
	GDREGISTER_CLASS(ParallaxLayer);
	GDREGISTER_CLASS(TouchScreenButton);
	GDREGISTER_CLASS(RemoteTransform2D);

	GDREGISTER_CLASS(SkeletonModificationStack2D);
	GDREGISTER_CLASS(SkeletonModification2D);
	GDREGISTER_CLASS(SkeletonModification2DLookAt);
	GDREGISTER_CLASS(SkeletonModification2DCCDIK);
	GDREGISTER_CLASS(SkeletonModification2DFABRIK);
	GDREGISTER_CLASS(SkeletonModification2DJiggle);
	GDREGISTER_CLASS(SkeletonModification2DTwoBoneIK);
	GDREGISTER_CLASS(SkeletonModification2DStackHolder);

	GDREGISTER_CLASS(PhysicalBone2D);
	GDREGISTER_CLASS(SkeletonModification2DPhysicalBones);

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER RESOURCES */

	GDREGISTER_VIRTUAL_CLASS(Shader);
	GDREGISTER_CLASS(ParticlesMaterial);
	SceneTree::add_idle_callback(ParticlesMaterial::flush_changes);
	ParticlesMaterial::init_shaders();

	GDREGISTER_CLASS(ProceduralSkyMaterial);
	GDREGISTER_CLASS(PanoramaSkyMaterial);
	GDREGISTER_CLASS(PhysicalSkyMaterial);

	GDREGISTER_VIRTUAL_CLASS(Mesh);
	GDREGISTER_CLASS(ArrayMesh);
	GDREGISTER_CLASS(ImmediateMesh);
	GDREGISTER_CLASS(MultiMesh);
	GDREGISTER_CLASS(SurfaceTool);
	GDREGISTER_CLASS(MeshDataTool);

#ifndef _3D_DISABLED
	GDREGISTER_VIRTUAL_CLASS(PrimitiveMesh);
	GDREGISTER_CLASS(BoxMesh);
	GDREGISTER_CLASS(CapsuleMesh);
	GDREGISTER_CLASS(CylinderMesh);
	GDREGISTER_CLASS(PlaneMesh);
	GDREGISTER_CLASS(PrismMesh);
	GDREGISTER_CLASS(QuadMesh);
	GDREGISTER_CLASS(SphereMesh);
	GDREGISTER_CLASS(TubeTrailMesh);
	GDREGISTER_CLASS(RibbonTrailMesh);
	GDREGISTER_CLASS(PointMesh);
	GDREGISTER_VIRTUAL_CLASS(Material);
	GDREGISTER_VIRTUAL_CLASS(BaseMaterial3D);
	GDREGISTER_CLASS(StandardMaterial3D);
	GDREGISTER_CLASS(ORMMaterial3D);
	SceneTree::add_idle_callback(BaseMaterial3D::flush_changes);
	BaseMaterial3D::init_shaders();

	GDREGISTER_CLASS(MeshLibrary);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_VIRTUAL_CLASS(Shape3D);
	GDREGISTER_CLASS(SeparationRayShape3D);
	GDREGISTER_CLASS(SphereShape3D);
	GDREGISTER_CLASS(BoxShape3D);
	GDREGISTER_CLASS(CapsuleShape3D);
	GDREGISTER_CLASS(CylinderShape3D);
	GDREGISTER_CLASS(HeightMapShape3D);
	GDREGISTER_CLASS(WorldBoundaryShape3D);
	GDREGISTER_CLASS(ConvexPolygonShape3D);
	GDREGISTER_CLASS(ConcavePolygonShape3D);

	ClassDB::register_class<SkeletonModificationStack3D>();
	ClassDB::register_class<SkeletonModification3D>();
	ClassDB::register_class<SkeletonModification3DLookAt>();
	ClassDB::register_class<SkeletonModification3DCCDIK>();
	ClassDB::register_class<SkeletonModification3DFABRIK>();
	ClassDB::register_class<SkeletonModification3DJiggle>();
	ClassDB::register_class<SkeletonModification3DTwoBoneIK>();
	ClassDB::register_class<SkeletonModification3DStackHolder>();

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(VelocityTracker3D);
#endif

	GDREGISTER_CLASS(PhysicsMaterial);
	GDREGISTER_CLASS(World3D);
	GDREGISTER_CLASS(Environment);
	GDREGISTER_CLASS(CameraEffects);
	GDREGISTER_CLASS(World2D);
	GDREGISTER_VIRTUAL_CLASS(Texture);
	GDREGISTER_VIRTUAL_CLASS(Texture2D);
	GDREGISTER_CLASS(Sky);
	GDREGISTER_CLASS(StreamTexture2D);
	GDREGISTER_CLASS(ImageTexture);
	GDREGISTER_CLASS(AtlasTexture);
	GDREGISTER_CLASS(MeshTexture);
	GDREGISTER_CLASS(CurveTexture);
	GDREGISTER_CLASS(CurveXYZTexture);
	GDREGISTER_CLASS(GradientTexture1D);
	GDREGISTER_CLASS(GradientTexture2D);
	GDREGISTER_CLASS(ProxyTexture);
	GDREGISTER_CLASS(AnimatedTexture);
	GDREGISTER_CLASS(CameraTexture);
	GDREGISTER_VIRTUAL_CLASS(TextureLayered);
	GDREGISTER_VIRTUAL_CLASS(ImageTextureLayered);
	GDREGISTER_VIRTUAL_CLASS(Texture3D);
	GDREGISTER_CLASS(ImageTexture3D);
	GDREGISTER_CLASS(StreamTexture3D);
	GDREGISTER_CLASS(Cubemap);
	GDREGISTER_CLASS(CubemapArray);
	GDREGISTER_CLASS(Texture2DArray);
	GDREGISTER_VIRTUAL_CLASS(StreamTextureLayered);
	GDREGISTER_CLASS(StreamCubemap);
	GDREGISTER_CLASS(StreamCubemapArray);
	GDREGISTER_CLASS(StreamTexture2DArray);

	GDREGISTER_CLASS(Animation);
	GDREGISTER_CLASS(FontData);
	GDREGISTER_CLASS(Font);
	GDREGISTER_CLASS(Curve);

	GDREGISTER_CLASS(SceneReplicationConfig);

	GDREGISTER_CLASS(TextLine);
	GDREGISTER_CLASS(TextParagraph);

	GDREGISTER_VIRTUAL_CLASS(StyleBox);
	GDREGISTER_CLASS(StyleBoxEmpty);
	GDREGISTER_CLASS(StyleBoxTexture);
	GDREGISTER_CLASS(StyleBoxFlat);
	GDREGISTER_CLASS(StyleBoxLine);
	GDREGISTER_CLASS(Theme);

	GDREGISTER_CLASS(PolygonPathFinder);
	GDREGISTER_CLASS(BitMap);
	GDREGISTER_CLASS(Gradient);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_CLASS(AudioStreamPlayer);
	GDREGISTER_CLASS(AudioStreamPlayer2D);
#ifndef _3D_DISABLED
	GDREGISTER_CLASS(AudioStreamPlayer3D);
#endif
	GDREGISTER_VIRTUAL_CLASS(VideoStream);
	GDREGISTER_CLASS(AudioStreamSample);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_VIRTUAL_CLASS(Shape2D);
	GDREGISTER_CLASS(WorldBoundaryShape2D);
	GDREGISTER_CLASS(SegmentShape2D);
	GDREGISTER_CLASS(SeparationRayShape2D);
	GDREGISTER_CLASS(CircleShape2D);
	GDREGISTER_CLASS(RectangleShape2D);
	GDREGISTER_CLASS(CapsuleShape2D);
	GDREGISTER_CLASS(ConvexPolygonShape2D);
	GDREGISTER_CLASS(ConcavePolygonShape2D);
	GDREGISTER_CLASS(Curve2D);
	GDREGISTER_CLASS(Path2D);
	GDREGISTER_CLASS(PathFollow2D);

	GDREGISTER_CLASS(NavigationMesh);
	GDREGISTER_CLASS(NavigationPolygon);
	GDREGISTER_CLASS(NavigationRegion2D);
	GDREGISTER_CLASS(NavigationAgent2D);
	GDREGISTER_CLASS(NavigationObstacle2D);

	OS::get_singleton()->yield(); // may take time to init

	GDREGISTER_VIRTUAL_CLASS(SceneState);
	GDREGISTER_CLASS(PackedScene);

	GDREGISTER_CLASS(SceneTree);
	GDREGISTER_VIRTUAL_CLASS(SceneTreeTimer); // sorry, you can't create it

#ifndef DISABLE_DEPRECATED
	// Dropped in 4.0, near approximation.
	ClassDB::add_compatibility_class("AnimationTreePlayer", "AnimationTree");
	ClassDB::add_compatibility_class("BitmapFont", "Font");
	ClassDB::add_compatibility_class("DynamicFont", "Font");
	ClassDB::add_compatibility_class("DynamicFontData", "FontData");
	ClassDB::add_compatibility_class("ToolButton", "Button");
	ClassDB::add_compatibility_class("Navigation3D", "Node3D");
	ClassDB::add_compatibility_class("Navigation2D", "Node2D");
	ClassDB::add_compatibility_class("YSort", "Node2D");
	ClassDB::add_compatibility_class("GIProbe", "VoxelGI");
	ClassDB::add_compatibility_class("GIProbeData", "VoxelGIData");
	ClassDB::add_compatibility_class("BakedLightmap", "LightmapGI");
	ClassDB::add_compatibility_class("BakedLightmapData", "LightmapGIData");
	// Portal and room occlusion was replaced by raster occlusion (OccluderInstance3D node).
	ClassDB::add_compatibility_class("Portal", "Node3D");
	ClassDB::add_compatibility_class("Room", "Node3D");
	ClassDB::add_compatibility_class("RoomManager", "Node3D");
	ClassDB::add_compatibility_class("RoomGroup", "Node3D");
	ClassDB::add_compatibility_class("Occluder", "Node3D");
	// The OccluderShapeSphere resource (used in the old Occluder node) is not present anymore.
	ClassDB::add_compatibility_class("OccluderShapeSphere", "Resource");

	// Renamed in 4.0.
	// Keep alphabetical ordering to easily locate classes and avoid duplicates.
	ClassDB::add_compatibility_class("AnimatedSprite", "AnimatedSprite2D");
	ClassDB::add_compatibility_class("Area", "Area3D");
	ClassDB::add_compatibility_class("ARVRCamera", "XRCamera3D");
	ClassDB::add_compatibility_class("ARVRController", "XRController3D");
	ClassDB::add_compatibility_class("ARVRAnchor", "XRAnchor3D");
	ClassDB::add_compatibility_class("ARVRInterface", "XRInterface");
	ClassDB::add_compatibility_class("ARVROrigin", "XROrigin3D");
	ClassDB::add_compatibility_class("ARVRPositionalTracker", "XRPositionalTracker");
	ClassDB::add_compatibility_class("ARVRServer", "XRServer");
	ClassDB::add_compatibility_class("BoneAttachment", "BoneAttachment3D");
	ClassDB::add_compatibility_class("BoxShape", "BoxShape3D");
	ClassDB::add_compatibility_class("BulletPhysicsDirectBodyState", "BulletPhysicsDirectBodyState3D");
	ClassDB::add_compatibility_class("BulletPhysicsServer", "BulletPhysicsServer3D");
	ClassDB::add_compatibility_class("Camera", "Camera3D");
	ClassDB::add_compatibility_class("CapsuleShape", "CapsuleShape3D");
	ClassDB::add_compatibility_class("ClippedCamera", "ClippedCamera3D");
	ClassDB::add_compatibility_class("CollisionObject", "CollisionObject3D");
	ClassDB::add_compatibility_class("CollisionPolygon", "CollisionPolygon3D");
	ClassDB::add_compatibility_class("CollisionShape", "CollisionShape3D");
	ClassDB::add_compatibility_class("ConcavePolygonShape", "ConcavePolygonShape3D");
	ClassDB::add_compatibility_class("ConeTwistJoint", "ConeTwistJoint3D");
	ClassDB::add_compatibility_class("ConvexPolygonShape", "ConvexPolygonShape3D");
	ClassDB::add_compatibility_class("CPUParticles", "CPUParticles3D");
	ClassDB::add_compatibility_class("CSGBox", "CSGBox3D");
	ClassDB::add_compatibility_class("CSGCombiner", "CSGCombiner3D");
	ClassDB::add_compatibility_class("CSGCylinder", "CSGCylinder3D");
	ClassDB::add_compatibility_class("CSGMesh", "CSGMesh3D");
	ClassDB::add_compatibility_class("CSGPolygon", "CSGPolygon3D");
	ClassDB::add_compatibility_class("CSGPrimitive", "CSGPrimitive3D");
	ClassDB::add_compatibility_class("CSGShape", "CSGShape3D");
	ClassDB::add_compatibility_class("CSGSphere", "CSGSphere3D");
	ClassDB::add_compatibility_class("CSGTorus", "CSGTorus3D");
	ClassDB::add_compatibility_class("CubeMesh", "BoxMesh");
	ClassDB::add_compatibility_class("CylinderShape", "CylinderShape3D");
	ClassDB::add_compatibility_class("DirectionalLight", "DirectionalLight3D");
	ClassDB::add_compatibility_class("EditorSpatialGizmo", "EditorNode3DGizmo");
	ClassDB::add_compatibility_class("EditorSpatialGizmoPlugin", "EditorNode3DGizmoPlugin");
	ClassDB::add_compatibility_class("Generic6DOFJoint", "Generic6DOFJoint3D");
	ClassDB::add_compatibility_class("GradientTexture", "GradientTexture1D");
	ClassDB::add_compatibility_class("HeightMapShape", "HeightMapShape3D");
	ClassDB::add_compatibility_class("HingeJoint", "HingeJoint3D");
	ClassDB::add_compatibility_class("Joint", "Joint3D");
	ClassDB::add_compatibility_class("KinematicBody", "CharacterBody3D");
	ClassDB::add_compatibility_class("KinematicBody2D", "CharacterBody2D");
	ClassDB::add_compatibility_class("KinematicCollision", "KinematicCollision3D");
	ClassDB::add_compatibility_class("Light", "Light3D");
	ClassDB::add_compatibility_class("Light2D", "PointLight2D");
	ClassDB::add_compatibility_class("LineShape2D", "WorldBoundaryShape2D");
	ClassDB::add_compatibility_class("Listener", "AudioListener3D");
	ClassDB::add_compatibility_class("MeshInstance", "MeshInstance3D");
	ClassDB::add_compatibility_class("MultiMeshInstance", "MultiMeshInstance3D");
	ClassDB::add_compatibility_class("NavigationAgent", "NavigationAgent3D");
	ClassDB::add_compatibility_class("NavigationMeshInstance", "NavigationRegion3D");
	ClassDB::add_compatibility_class("NavigationObstacle", "NavigationObstacle3D");
	ClassDB::add_compatibility_class("NavigationPolygonInstance", "NavigationRegion2D");
	ClassDB::add_compatibility_class("NavigationRegion", "NavigationRegion3D");
	ClassDB::add_compatibility_class("Navigation2DServer", "NavigationServer2D");
	ClassDB::add_compatibility_class("NavigationServer", "NavigationServer3D");
	ClassDB::add_compatibility_class("OmniLight", "OmniLight3D");
	ClassDB::add_compatibility_class("PanoramaSky", "Sky");
	ClassDB::add_compatibility_class("Particles", "GPUParticles3D");
	ClassDB::add_compatibility_class("Particles2D", "GPUParticles2D");
	ClassDB::add_compatibility_class("Path", "Path3D");
	ClassDB::add_compatibility_class("PathFollow", "PathFollow3D");
	ClassDB::add_compatibility_class("PhysicalBone", "PhysicalBone3D");
	ClassDB::add_compatibility_class("Physics2DDirectBodyState", "PhysicsDirectBodyState2D");
	ClassDB::add_compatibility_class("Physics2DDirectSpaceState", "PhysicsDirectSpaceState2D");
	ClassDB::add_compatibility_class("Physics2DServer", "PhysicsServer2D");
	ClassDB::add_compatibility_class("Physics2DShapeQueryParameters", "PhysicsShapeQueryParameters2D");
	ClassDB::add_compatibility_class("Physics2DTestMotionResult", "PhysicsTestMotionResult2D");
	ClassDB::add_compatibility_class("PhysicsBody", "PhysicsBody3D");
	ClassDB::add_compatibility_class("PhysicsDirectBodyState", "PhysicsDirectBodyState3D");
	ClassDB::add_compatibility_class("PhysicsDirectSpaceState", "PhysicsDirectSpaceState3D");
	ClassDB::add_compatibility_class("PhysicsServer", "PhysicsServer3D");
	ClassDB::add_compatibility_class("PhysicsShapeQueryParameters", "PhysicsShapeQueryParameters3D");
	ClassDB::add_compatibility_class("PinJoint", "PinJoint3D");
	ClassDB::add_compatibility_class("PlaneShape", "WorldBoundaryShape3D");
	ClassDB::add_compatibility_class("ProceduralSky", "Sky");
	ClassDB::add_compatibility_class("RayCast", "RayCast3D");
	ClassDB::add_compatibility_class("RayShape", "SeparationRayShape3D");
	ClassDB::add_compatibility_class("RayShape2D", "SeparationRayShape2D");
	ClassDB::add_compatibility_class("RemoteTransform", "RemoteTransform3D");
	ClassDB::add_compatibility_class("RigidBody", "RigidDynamicBody3D");
	ClassDB::add_compatibility_class("RigidBody2D", "RigidDynamicBody2D");
	ClassDB::add_compatibility_class("Shape", "Shape3D");
	ClassDB::add_compatibility_class("ShortCut", "Shortcut");
	ClassDB::add_compatibility_class("Skeleton", "Skeleton3D");
	ClassDB::add_compatibility_class("SkeletonIK", "SkeletonIK3D");
	ClassDB::add_compatibility_class("SliderJoint", "SliderJoint3D");
	ClassDB::add_compatibility_class("SoftBody", "SoftDynamicBody3D");
	ClassDB::add_compatibility_class("Spatial", "Node3D");
	ClassDB::add_compatibility_class("SpatialGizmo", "Node3DGizmo");
	ClassDB::add_compatibility_class("SpatialMaterial", "StandardMaterial3D");
	ClassDB::add_compatibility_class("SpatialVelocityTracker", "VelocityTracker3D");
	ClassDB::add_compatibility_class("SphereShape", "SphereShape3D");
	ClassDB::add_compatibility_class("SpotLight", "SpotLight3D");
	ClassDB::add_compatibility_class("SpringArm", "SpringArm3D");
	ClassDB::add_compatibility_class("Sprite", "Sprite2D");
	ClassDB::add_compatibility_class("StaticBody", "StaticBody3D");
	ClassDB::add_compatibility_class("StreamTexture", "StreamTexture2D");
	ClassDB::add_compatibility_class("TextureProgress", "TextureProgressBar");
	ClassDB::add_compatibility_class("VehicleBody", "VehicleBody3D");
	ClassDB::add_compatibility_class("VehicleWheel", "VehicleWheel3D");
	ClassDB::add_compatibility_class("VideoPlayer", "VideoStreamPlayer");
	ClassDB::add_compatibility_class("ViewportContainer", "SubViewportContainer");
	ClassDB::add_compatibility_class("Viewport", "SubViewport");
	ClassDB::add_compatibility_class("VisibilityEnabler", "VisibleOnScreenEnabler3D");
	ClassDB::add_compatibility_class("VisibilityNotifier", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisibilityNotifier2D", "VisibleOnScreenNotifier2D");
	ClassDB::add_compatibility_class("VisibilityNotifier3D", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisualServer", "RenderingServer");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarConstant", "VisualShaderNodeFloatConstant");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarFunc", "VisualShaderNodeFloatFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarOp", "VisualShaderNodeFloatOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarUniform", "VisualShaderNodeFloatUniform");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarMix", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarStep", "VisualShaderNodeStep");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSwitch", "VisualShaderNodeSwitch");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarTransformMult", "VisualShaderNodeTransformOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarDerivativeFunc", "VisualShaderNodeDerivativeFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorDerivativeFunc", "VisualShaderNodeDerivativeFunc");
	ClassDB::add_compatibility_class("World", "World3D");
#endif /* DISABLE_DEPRECATED */

	OS::get_singleton()->yield(); // may take time to init

	for (int i = 0; i < 20; i++) {
		GLOBAL_DEF_BASIC(vformat("layer_names/2d_render/layer_%d", i + 1), "");
		GLOBAL_DEF_BASIC(vformat("layer_names/3d_render/layer_%d", i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF_BASIC(vformat("layer_names/2d_physics/layer_%d", i + 1), "");
		GLOBAL_DEF_BASIC(vformat("layer_names/2d_navigation/layer_%d", i + 1), "");
		GLOBAL_DEF_BASIC(vformat("layer_names/3d_physics/layer_%d", i + 1), "");
		GLOBAL_DEF_BASIC(vformat("layer_names/3d_navigation/layer_%d", i + 1), "");
	}

	if (RenderingServer::get_singleton()) {
		ColorPicker::init_shaders(); // RenderingServer needs to exist for this to succeed.
	}

	SceneDebugger::initialize();
	SceneReplicationInterface::make_default();
	SceneRPCInterface::make_default();
	SceneCacheInterface::make_default();

	NativeExtensionManager::get_singleton()->initialize_extensions(NativeExtension::INITIALIZATION_LEVEL_SCENE);
}

void initialize_theme() {
	// Allow creating the default theme at a different scale to suit higher/lower base resolutions.
	float default_theme_scale = GLOBAL_DEF("gui/theme/default_theme_scale", 1.0);
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/default_theme_scale", PropertyInfo(Variant::FLOAT, "gui/theme/default_theme_scale", PROPERTY_HINT_RANGE, "0.5,8,0.01", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));
	String theme_path = GLOBAL_DEF_RST("gui/theme/custom", "");
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/custom", PropertyInfo(Variant::STRING, "gui/theme/custom", PROPERTY_HINT_FILE, "*.tres,*.res,*.theme", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));
	String font_path = GLOBAL_DEF_RST("gui/theme/custom_font", "");
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/custom_font", PropertyInfo(Variant::STRING, "gui/theme/custom_font", PROPERTY_HINT_FILE, "*.tres,*.res,*.font", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));

	Ref<Font> font;
	if (!font_path.is_empty()) {
		font = ResourceLoader::load(font_path);
		if (!font.is_valid()) {
			ERR_PRINT("Error loading custom font '" + font_path + "'");
		}
	}

	// Always make the default theme to avoid invalid default font/icon/style in the given theme.
	if (RenderingServer::get_singleton()) {
		make_default_theme(default_theme_scale, font);
	}

	if (!theme_path.is_empty()) {
		Ref<Theme> theme = ResourceLoader::load(theme_path);
		if (theme.is_valid()) {
			Theme::set_project_default(theme);
			if (font.is_valid()) {
				Theme::set_fallback_font(font);
			}
		} else {
			ERR_PRINT("Error loading custom theme '" + theme_path + "'");
		}
	}
}

void unregister_scene_types() {
	NativeExtensionManager::get_singleton()->deinitialize_extensions(NativeExtension::INITIALIZATION_LEVEL_SCENE);

	SceneDebugger::deinitialize();
	clear_default_theme();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_layered);
	resource_loader_texture_layered.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_3d);
	resource_loader_texture_3d.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_stream_texture);
	resource_loader_stream_texture.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_text);
	resource_saver_text.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_text);
	resource_loader_text.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader);
	resource_saver_shader.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader);
	resource_loader_shader.unref();

	// StandardMaterial3D is not initialised when 3D is disabled, so it shouldn't be cleaned up either
#ifndef _3D_DISABLED
	BaseMaterial3D::finish_shaders();
#endif // _3D_DISABLED

	PhysicalSkyMaterial::cleanup_shader();
	PanoramaSkyMaterial::cleanup_shader();
	ProceduralSkyMaterial::cleanup_shader();

	ParticlesMaterial::finish_shaders();
	CanvasItemMaterial::finish_shaders();
	ColorPicker::finish_shaders();
	SceneStringNames::free();
}
