- [Getting Started](#getting-started)
  - [Compiling libdiscord for Windows](#compiling-libdiscord-for-windows)
  - [Compiling a bot on Windows](#compiling-a-bot-on-windows)
  - [Compiling libdiscord for Linux](#compiling-libdiscord-for-linux)
  - [Compiling a bot on Linux](#compiling-a-bot-on-linux)
- [Examples](#examples)
  - [Handling OnMessage](#handling-onmessage)
  - [Creating a Command](#creating-a-command)
  
## Getting Started

### Compiling Library for Windows

You should be able to simply download this git repository and open up the .sln file and build it after installing some dependencies. 

The solution was made in Visual Studio 2015 and has the following dependencies:

- **CPPRestSDK** You can install this as a Nuget package.
- **ZLib** You can install this as a Nuget package. 

After installing these dependencies you should be able to compile the libdiscord project.

### Compiling a Bot on Windows
To compile a bot on Windows you must have libdiscord.lib and the libdiscord include directory in your path. If using Visual Studio, simply add them to your project settings. After your project is set up, you must include the `mod_discord.h` header file to access the library.

### Compiling Library for Linux
The git project has a very simple makefile, so you should be able to just use make in the root directory. However, you will need to have CPPRestSDK and its dependencies installed. Visit [this link](https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux) to learn how to install it.

Here is a list of all the libraries you must have installed:

- boost_system
- cpprest
- crypto
- pthread
- ssl
- z

### Compiling a Bot on Linux
This is a bit more involved than Windows simply because I don't know if you can combine shared libraries easily. Assuming you have a project with a single `main.cpp` file, you would compile it like so:

```
g++ main.cpp -Llibdiscord/bin -Ilibdiscord/include -ldiscord -lboost_system -lcrypto -lssl -lcpprest -lz -lpthread -std=c++14 -o output_file
```

In here, you should be using the -L flag to add the path where libdiscord.so is stored. Likewise, -I should be the path to the libdiscord includes. After that, you must include -ldiscord for this library, and then link it against all its dependencies as well. This is a bit more than I would like for a simple program, so I'll try to look into simplifying it later.

## Examples
### Handling OnMessage
The following bot will respond to any `Ping!` with a `Pong!`

```cpp
#include <discord.h>

//  Use this to avoid using Discord:: everywhere
//  using namespace Discord;

//  Or use this to shorten it to just ds::
//  namespace ds = Discord;

int main() {
  std::string token = "YOUR_TOKEN";
  auto bot = Discord::Bot::create(token);
  
  bot->on_message([](std::shared_ptr<Discord::MessageEvent> event) {
    if (event->content() == "Ping!") {
      event->respond("Pong!");
    }
  });
  
  bot->run();
}
```

### Creating a Command
Creating a command is just like handing `on_message`, except it takes a command name parameter.

```cpp
#include <discord.h>

int main() {
  std::string token = "YOUR_TOKEN";
  auto bot = Discord::Bot::create(token, "!"); // ! is the prefix
  
  //  This command will respond "Pong!" to any "!ping"
  bot->add_command("ping", [](std::shared_ptr<Discord::MessageEvent> event) {
    event->respond("Pong!");
  });
  
  bot->run();
}
```
