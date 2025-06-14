# SceneViewExtensionTemplate

Unreal Engine 5 plugin template for adding a custom rending pass into the engine with a SceneViewExtension

### Features

- Basic Compute Shader to test and showcase functionality (swaps R and B channels of the SceneColor)
- Engine Subsystem module to keep the custom SceneViewExtension alive
- Simplified but documented SceneViewExtension code to explain the main functions

### Installation and use

- Install SceneViewExtensionTemplate to projects plugin directory and enable the plugin
- Use console command r.SceneViewExtensionTemplate 1 to activate SceneViewExtension

### Motivation

This repository showcases a simple way to implement custom rendering pass to Unreal Engine without modifying the engine code. This utilizes the engine feature called SceneViewExtension that enables to hook into various phases of the rendering pipeline and an engine subsystem to keep a single instance of the extension alive.

This is a rewrite of my previous repository to update the codebase to Unreal Engine 5 and to be able to release the code under MIT license. At the time of writing the previous project, I didn't find a proper documentation on how to implement a custom pass to the rendering pipeline without modifying the engine code. You can find more information on the background from the legacy project for UE 4.26 - 5.01. [Link to the legacy SceneViewExtTest project](https://github.com/A57R4L/SceneViewExtTest)

### License

This repository is licensed under the terms of the MIT license

For the sake of clarity, this repository extends Unreal Engine classes. For more information and license on using Unreal Engine please refer to: [Unreal Engine download page](https://www.unrealengine.com/download)

### Additional notes

I have tried to keep the code as simple as possible and document the most relevant features in code comments. I try to follow the updates to Unreal Engine and update the repository accordingly. If you have further questions or suggestions on how to improve this, please don' hesitate to contact me or contribute through pull request.

### Updates

June 2025: Update for Unreal Engine 5.6, clean up code and simplify the example setup, move 5.0-5.5 to legacy branch
October 2024: Added update/fixes for Unreal Engine 5.5-preview
June 2024: Update the template for Unreal Engine 5.4