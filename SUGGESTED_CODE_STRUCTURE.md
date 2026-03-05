# Suggested Code Structure
- each component has it's own object
	- each object has a periodic/update function with no parameters
	- Each object has an initialization function that will be run on setup. These can include parameters if needed.
	- each object has a set of state variables, changed either internally via public functions or by direct access

- main loop iterates through a **vector** (not array! different things)	of functions to run. If a periodic function doesn't need to be run, it is not included in the vector. By omitting unneeded functions, such as the display being off or not having audio playing, we will save cpu cycles, leading to a snappier user experience.
- Might be done by default, but audio processing should be delegated to a separate thread.