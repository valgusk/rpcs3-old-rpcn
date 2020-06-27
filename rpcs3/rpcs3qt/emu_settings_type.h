﻿#pragma once

#include <QMap>

// Node location
using cfg_location = std::vector<const char*>;

enum class emu_settings_type
{
	// Core
	PPUDecoder,
	SPUDecoder,
	LibLoadOptions,
	HookStaticFuncs,
	EnableThreadScheduler,
	LowerSPUThreadPrio,
	SPULoopDetection,
	PreferredSPUThreads,
	PPUDebug,
	SPUDebug,
	MaxLLVMThreads,
	EnableTSX,
	AccurateGETLLAR,
	AccuratePUTLLUC,
	AccurateLLVMdfma,
	AccurateVectorNaN,
	AccurateRSXAccess,
	AccurateXFloat,
	SetDAZandFTZ,
	SPUBlockSize,
	SPUCache,
	DebugConsoleMode,
	SilenceAllLogs,
	MaxSPURSThreads,
	SleepTimersAccuracy,
	ClocksScale,

	// Graphics
	Renderer,
	Resolution,
	AspectRatio,
	FrameLimit,
	MSAA,
	LogShaderPrograms,
	WriteDepthBuffer,
	WriteColorBuffers,
	ReadColorBuffers,
	ReadDepthBuffer,
	VSync,
	DebugOutput,
	DebugOverlay,
	LegacyBuffers,
	GPUTextureScaling,
	StretchToDisplayArea,
	VulkanAdapter,
	ForceHighpZ,
	StrictRenderingMode,
	DisableVertexCache,
	DisableOcclusionQueries,
	DisableFIFOReordering,
	StrictTextureFlushing,
	DisableNativefloat16,
	Enable3D,
	AnisotropicFilterOverride,
	ResolutionScale,
	MinimumScalableDimension,
	ForceCPUBlitEmulation,
	DisableOnDiskShaderCache,
	DisableVulkanMemAllocator,
	ShaderMode,
	MultithreadedRSX,
	VBlankRate,
	RelaxedZCULL,
	DriverWakeUpDelay,

	// Performance Overlay
	PerfOverlayEnabled,
	PerfOverlayFramerateGraphEnabled,
	PerfOverlayFrametimeGraphEnabled,
	PerfOverlayDetailLevel,
	PerfOverlayPosition,
	PerfOverlayUpdateInterval,
	PerfOverlayFontSize,
	PerfOverlayOpacity,
	PerfOverlayMarginX,
	PerfOverlayMarginY,
	PerfOverlayCenterX,
	PerfOverlayCenterY,

	// Shader Loading Dialog
	ShaderLoadBgEnabled,
	ShaderLoadBgDarkening,
	ShaderLoadBgBlur,

	// Audio
	AudioRenderer,
	DumpToFile,
	ConvertTo16Bit,
	DownmixStereo,
	MasterVolume,
	EnableBuffering,
	AudioBufferDuration,
	EnableTimeStretching,
	TimeStretchingThreshold,
	MicrophoneType,
	MicrophoneDevices,

	// Input / Output
	KeyboardHandler,
	MouseHandler,
	Camera,
	CameraType,
	Move,

	// Misc
	ExitRPCS3OnFinish,
	StartOnBoot,
	StartGameFullscreen,
	PreventDisplaySleep,
	ShowTrophyPopups,
	UseNativeInterface,
	ShowShaderCompilationHint,
	WindowTitleFormat,

	// Network
	InternetStatus,
	DNSAddress,
	IpSwapList,
	PSNStatus,
	PSNNPID,
	RPCNHost,
	RPCNPassword,

	// System
	Language,
	KeyboardType,
	EnterButtonAssignment,
	EnableHostRoot,
	LimitCacheSize,
	MaximumCacheSize,

	// Virtual File System
	emulatorLocation,
	dev_hdd0Location,
	dev_hdd1Location,
	dev_flashLocation,
	dev_usb000Location,
};

/** A helper map that keeps track of where a given setting type is located*/
static const QMap<emu_settings_type, cfg_location> settings_location =
{
	// Core Tab
	{ emu_settings_type::PPUDecoder,               { "Core", "PPU Decoder"}},
	{ emu_settings_type::SPUDecoder,               { "Core", "SPU Decoder"}},
	{ emu_settings_type::LibLoadOptions,           { "Core", "Lib Loader"}},
	{ emu_settings_type::HookStaticFuncs,          { "Core", "Hook static functions"}},
	{ emu_settings_type::EnableThreadScheduler,    { "Core", "Enable thread scheduler"}},
	{ emu_settings_type::LowerSPUThreadPrio,       { "Core", "Lower SPU thread priority"}},
	{ emu_settings_type::SPULoopDetection,         { "Core", "SPU loop detection"}},
	{ emu_settings_type::PreferredSPUThreads,      { "Core", "Preferred SPU Threads"}},
	{ emu_settings_type::PPUDebug,                 { "Core", "PPU Debug"}},
	{ emu_settings_type::SPUDebug,                 { "Core", "SPU Debug"}},
	{ emu_settings_type::MaxLLVMThreads,           { "Core", "Max LLVM Compile Threads"}},
	{ emu_settings_type::EnableTSX,                { "Core", "Enable TSX"}},
	{ emu_settings_type::AccurateGETLLAR,          { "Core", "Accurate GETLLAR"}},
	{ emu_settings_type::AccuratePUTLLUC,          { "Core", "Accurate PUTLLUC"}},
	{ emu_settings_type::AccurateLLVMdfma,         { "Core", "LLVM Accurate DFMA"}},
	{ emu_settings_type::AccurateVectorNaN,        { "Core", "PPU LLVM Accurate Vector NaN values"}},
	{ emu_settings_type::AccurateRSXAccess,        { "Core", "Accurate RSX reservation access"}},
	{ emu_settings_type::AccurateXFloat,           { "Core", "Accurate xfloat"}},
	{ emu_settings_type::SetDAZandFTZ,             { "Core", "Set DAZ and FTZ"}},
	{ emu_settings_type::SPUBlockSize,             { "Core", "SPU Block Size"}},
	{ emu_settings_type::SPUCache,                 { "Core", "SPU Cache"}},
	{ emu_settings_type::DebugConsoleMode,         { "Core", "Debug Console Mode"}},
	{ emu_settings_type::MaxSPURSThreads,          { "Core", "Max SPURS Threads"}},
	{ emu_settings_type::SleepTimersAccuracy,      { "Core", "Sleep Timers Accuracy"}},
	{ emu_settings_type::ClocksScale,              { "Core", "Clocks scale"}},

	// Graphics Tab
	{ emu_settings_type::Renderer,                   { "Video", "Renderer"}},
	{ emu_settings_type::Resolution,                 { "Video", "Resolution"}},
	{ emu_settings_type::AspectRatio,                { "Video", "Aspect ratio"}},
	{ emu_settings_type::FrameLimit,                 { "Video", "Frame limit"}},
	{ emu_settings_type::MSAA,                       { "Video", "MSAA"}},
	{ emu_settings_type::LogShaderPrograms,          { "Video", "Log shader programs"}},
	{ emu_settings_type::WriteDepthBuffer,           { "Video", "Write Depth Buffer"}},
	{ emu_settings_type::WriteColorBuffers,          { "Video", "Write Color Buffers"}},
	{ emu_settings_type::ReadColorBuffers,           { "Video", "Read Color Buffers"}},
	{ emu_settings_type::ReadDepthBuffer,            { "Video", "Read Depth Buffer"}},
	{ emu_settings_type::VSync,                      { "Video", "VSync"}},
	{ emu_settings_type::DebugOutput,                { "Video", "Debug output"}},
	{ emu_settings_type::DebugOverlay,               { "Video", "Debug overlay"}},
	{ emu_settings_type::LegacyBuffers,              { "Video", "Use Legacy OpenGL Buffers"}},
	{ emu_settings_type::GPUTextureScaling,          { "Video", "Use GPU texture scaling"}},
	{ emu_settings_type::StretchToDisplayArea,       { "Video", "Stretch To Display Area"}},
	{ emu_settings_type::ForceHighpZ,                { "Video", "Force High Precision Z buffer"}},
	{ emu_settings_type::StrictRenderingMode,        { "Video", "Strict Rendering Mode"}},
	{ emu_settings_type::DisableVertexCache,         { "Video", "Disable Vertex Cache"}},
	{ emu_settings_type::DisableOcclusionQueries,    { "Video", "Disable ZCull Occlusion Queries"}},
	{ emu_settings_type::DisableFIFOReordering,      { "Video", "Disable FIFO Reordering"}},
	{ emu_settings_type::DisableNativefloat16,       { "Video", "Disable native float16 support"}},
	{ emu_settings_type::Enable3D,                   { "Video", "Enable 3D"}},
	{ emu_settings_type::StrictTextureFlushing,      { "Video", "Strict Texture Flushing"}},
	{ emu_settings_type::ForceCPUBlitEmulation,      { "Video", "Force CPU Blit"}},
	{ emu_settings_type::DisableOnDiskShaderCache,   { "Video", "Disable On-Disk Shader Cache"}},
	{ emu_settings_type::DisableVulkanMemAllocator,  { "Video", "Disable Vulkan Memory Allocator"}},
	{ emu_settings_type::ShaderMode,                 { "Video", "Shader Mode"}},
	{ emu_settings_type::MultithreadedRSX,           { "Video", "Multithreaded RSX"}},
	{ emu_settings_type::RelaxedZCULL,               { "Video", "Relaxed ZCULL Sync"}},
	{ emu_settings_type::AnisotropicFilterOverride,  { "Video", "Anisotropic Filter Override"}},
	{ emu_settings_type::ResolutionScale,            { "Video", "Resolution Scale"}},
	{ emu_settings_type::MinimumScalableDimension,   { "Video", "Minimum Scalable Dimension"}},
	{ emu_settings_type::VulkanAdapter,              { "Video", "Vulkan", "Adapter"}},
	{ emu_settings_type::VBlankRate,                 { "Video", "Vblank Rate"}},
	{ emu_settings_type::DriverWakeUpDelay,          { "Video", "Driver Wake-Up Delay"}},

	// Performance Overlay
	{ emu_settings_type::PerfOverlayEnabled,               { "Video", "Performance Overlay", "Enabled" } },
	{ emu_settings_type::PerfOverlayFramerateGraphEnabled, { "Video", "Performance Overlay", "Enable Framerate Graph" } },
	{ emu_settings_type::PerfOverlayFrametimeGraphEnabled, { "Video", "Performance Overlay", "Enable Frametime Graph" } },
	{ emu_settings_type::PerfOverlayDetailLevel,           { "Video", "Performance Overlay", "Detail level" } },
	{ emu_settings_type::PerfOverlayPosition,              { "Video", "Performance Overlay", "Position" } },
	{ emu_settings_type::PerfOverlayUpdateInterval,        { "Video", "Performance Overlay", "Metrics update interval (ms)" } },
	{ emu_settings_type::PerfOverlayFontSize,              { "Video", "Performance Overlay", "Font size (px)" } },
	{ emu_settings_type::PerfOverlayOpacity,               { "Video", "Performance Overlay", "Opacity (%)" } },
	{ emu_settings_type::PerfOverlayMarginX,               { "Video", "Performance Overlay", "Horizontal Margin (px)" } },
	{ emu_settings_type::PerfOverlayMarginY,               { "Video", "Performance Overlay", "Vertical Margin (px)" } },
	{ emu_settings_type::PerfOverlayCenterX,               { "Video", "Performance Overlay", "Center Horizontally" } },
	{ emu_settings_type::PerfOverlayCenterY,               { "Video", "Performance Overlay", "Center Vertically" } },

	// Shader Loading Dialog
	{ emu_settings_type::ShaderLoadBgEnabled,      { "Video", "Shader Loading Dialog", "Allow custom background" } },
	{ emu_settings_type::ShaderLoadBgDarkening,    { "Video", "Shader Loading Dialog", "Darkening effect strength" } },
	{ emu_settings_type::ShaderLoadBgBlur,         { "Video", "Shader Loading Dialog", "Blur effect strength" } },

	// Audio
	{ emu_settings_type::AudioRenderer,           { "Audio", "Renderer"}},
	{ emu_settings_type::DumpToFile,              { "Audio", "Dump to file"}},
	{ emu_settings_type::ConvertTo16Bit,          { "Audio", "Convert to 16 bit"}},
	{ emu_settings_type::DownmixStereo,           { "Audio", "Downmix to Stereo"}},
	{ emu_settings_type::MasterVolume,            { "Audio", "Master Volume"}},
	{ emu_settings_type::EnableBuffering,         { "Audio", "Enable Buffering"}},
	{ emu_settings_type::AudioBufferDuration,     { "Audio", "Desired Audio Buffer Duration"}},
	{ emu_settings_type::EnableTimeStretching,    { "Audio", "Enable Time Stretching"}},
	{ emu_settings_type::TimeStretchingThreshold, { "Audio", "Time Stretching Threshold"}},
	{ emu_settings_type::MicrophoneType,          { "Audio", "Microphone Type" }},
	{ emu_settings_type::MicrophoneDevices,       { "Audio", "Microphone Devices" }},

	// Input / Output
	{ emu_settings_type::KeyboardHandler, { "Input/Output", "Keyboard"}},
	{ emu_settings_type::MouseHandler,    { "Input/Output", "Mouse"}},
	{ emu_settings_type::Camera,          { "Input/Output", "Camera"}},
	{ emu_settings_type::CameraType,      { "Input/Output", "Camera type"}},
	{ emu_settings_type::Move,            { "Input/Output", "Move" }},

	// Misc
	{ emu_settings_type::ExitRPCS3OnFinish,         { "Miscellaneous", "Exit RPCS3 when process finishes" }},
	{ emu_settings_type::StartOnBoot,               { "Miscellaneous", "Automatically start games after boot" }},
	{ emu_settings_type::StartGameFullscreen,       { "Miscellaneous", "Start games in fullscreen mode"}},
	{ emu_settings_type::PreventDisplaySleep,       { "Miscellaneous", "Prevent display sleep while running games"}},
	{ emu_settings_type::ShowTrophyPopups,          { "Miscellaneous", "Show trophy popups"}},
	{ emu_settings_type::UseNativeInterface,        { "Miscellaneous", "Use native user interface"}},
	{ emu_settings_type::ShowShaderCompilationHint, { "Miscellaneous", "Show shader compilation hint"}},
	{ emu_settings_type::SilenceAllLogs,            { "Miscellaneous", "Silence All Logs" }},
	{ emu_settings_type::WindowTitleFormat,         { "Miscellaneous", "Window Title Format" }},

	// Networking
	{ emu_settings_type::InternetStatus, { "Net", "Internet enabled"}},
	{ emu_settings_type::DNSAddress,     { "Net", "DNS address"}},
	{ emu_settings_type::IpSwapList,     { "Net", "IP swap list"}},
	{ emu_settings_type::PSNStatus,      { "Net", "PSN status"}},
	{ emu_settings_type::PSNNPID,        { "Net", "NPID"}},
	{ emu_settings_type::RPCNHost,       { "Net", "RPCN host"}},
	{ emu_settings_type::RPCNPassword,   { "Net", "RPCN Password"}},

	// System
	{ emu_settings_type::Language,              { "System", "Language"}},
	{ emu_settings_type::KeyboardType,          { "System", "Keyboard Type"} },
	{ emu_settings_type::EnterButtonAssignment, { "System", "Enter button assignment"}},
	{ emu_settings_type::EnableHostRoot,        { "VFS", "Enable /host_root/"}},
	{ emu_settings_type::LimitCacheSize,        { "VFS", "Limit disk cache size"}},
	{ emu_settings_type::MaximumCacheSize,      { "VFS", "Disk cache maximum size (MB)"}},

	// Virtual File System
	{ emu_settings_type::emulatorLocation,   { "VFS", "$(EmulatorDir)"}},
	{ emu_settings_type::dev_hdd0Location,   { "VFS", "/dev_hdd0/" }},
	{ emu_settings_type::dev_hdd1Location,   { "VFS", "/dev_hdd1/" }},
	{ emu_settings_type::dev_flashLocation,  { "VFS", "/dev_flash/"}},
	{ emu_settings_type::dev_usb000Location, { "VFS", "/dev_usb000/"}},
};
