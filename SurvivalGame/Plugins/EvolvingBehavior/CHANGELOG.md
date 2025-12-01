# Changelog
Describes notable changes to EvolvingBehavior between versions.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

## [0.2.0] - 2022-08-09

NOTE: This release is not compatible with save files from earlier releases.

### Added
- Blueprint bindings added for all functions required to set up and run an experiment. (The example project now has a map that shows how to run an experiment entirely in Blueprint.)
- Support for evolving multiple separate populations at once. Each population is controlled by a separate EvolutionControlActor.
- EvolutionControlActors can now be looked up by Name or Tag, so NPCs can request behavior trees from a specific population.
- EvolutionControlActor now provides convenience functions for easy external access to functionality.
- EvolutionControlActor will automatically add a LifeCycleActor when put in the map, if there was not already a LifeCycleActor in the level.
- Added Blackboard Key property support for generated nodes. You can now generate Blackboard Keys as properties, where they will automatically mutate to use any available key of the correct type in a given Blackboard.
- Added mutators that randomly perturb property values for generated nodes (with gaussian distribution).
- Added Blueprint version of MoveTo Behavior Tree node, as an example of subclassing a node to allow it to have generated properties for EvolvingBehavior.
- Added Elitism option for keeping some of the best population members from each generation as exact copies in the next generation, to help avoid forgetting good behavior due to mutation.
- Added "Grid Search" capability, to provide for running multiple versions of an experiment with different parameter values (such as mutation rates), or just re-running multiple times with random starting populations. This can help with parameter tuning, or provide greater variety in results. To use this functionality, you can add the GridSearchExperimentManagementActor to your scene and configure it.

### Changed
- Updated to Unreal Engine 4.27 (should also be compatible with 4.26)
- API for finding an EvolutionControlActor has changed, as there can now be more than one.
- LifeCycleComponent changed to LifeCycleActor, now required to be placed in the scene separately from EvolutionControlActor. There should only be one LifeCycleActor per level (but there can be more than one EvolutionControlActor to evolve multiple populations).
- We now save populations separately from the main save file, and only load the populations we need, to significantly improve speed of evolution when there are a large number of generations.
- Switched to in-game instead of wall-clock time for the length of each trial (so you can keep the same length even if you change the game speed).
- Improved documentation with additional setup and troubleshooting info.
- Removed unnecessary logging.
- Added ability to release and re-register population members, so you can respawn them multiple times during a trial.
- "Delete save" button in LifeCycle is now a real button, rather than a checkbox.

### Fixed
- Fixed bug that could lead to a trial never stopping.
- Fixed bug causing some populations to never spawn.
- Fixed bugs with depth-biased crossover mutation that prevented it from working correctly and could cause infinite hanging.
- Fixed bug causing first member of first generation population to fail to spawn.
- Fixed bugs causing properties to sometimes load defaults instead of pre-set values in various components.
- Fixed bugs with copying service and decorator nodes.
- Fixed infinite loop for populations with only 1 member.

## [0.1.0] - 2020-12-03
### Added
- First released version!

