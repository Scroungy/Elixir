# Elixir

Vulkan based game engine

## Building

1. Open the folder in vscode

2. Run the build ccrt debug task (ctrl + shift + b) to build the debug crt (run build ccrt release for the non-debug version)

3. Run the grab libs task (yes to overwrite if asked)

4. Run the build debug or build release task

5. You can launch the exe by selecting a launch configuration or by just running it normally

## Notes

-   The current "error handling" is to just exit the application if something doesn't go right:
    -   resource creation fails during initializing vulkan resources
    -   a discrete gpu is not found
    -   required extensions don't exist
    -   vulkan version isn't 1.3
    -   no suitable gpu is found
    -   .spv files are not found in the exe directory
