## File format
File is binary. It represents one or more **Objects**.

Each **Object** begins with an **Object ID**, which is a `uint8_t` having one of the values of `SavedObjId` enum. **Object** can have multiple IDs, with first one specifying the base class and each following ID specifying the derived class. **Object ID** of base class always has its MSB set, while **Object ID** of the final class does not. This can be used to determine the final class when skipping objects. ID of 0 is reserved in all classes. If deserializer function encounters 0 ID of top class, it should exit, while leaving data pointer pointing at 0 (for handling early end of data)

Object data immediately follows object ID. It consists of one or more **data sections** It starts again with data section corresponding to base class and then for each of the derived classes in the hierarchy.

Each **data section** starts with `uint16_t` specifying its **length** (including length field). Then follows the length-2 bytes, containing fixed length data member info for the object. If, when deserializing, this length is less than actual size of data neeeded for the object, it is assumed that this is because file is creted by old version of a program and fields not present were not added yet, so they are initialized with some default values.

After the data section may follow one or more additional **Object**s, forming variable length data. Last byte of **data section** is always set to 0, to indicate that there are no additional objects present.

## Deserialization

When loading an **Object** from file, multiple ways to do that are used depending on **Object** type.

When loading a simple object like `PhysicalBody`, appropriate Deserialize function should be called with data source as an only argument. The resulting object is allocated using `new`, constructed, and returned from a function.

If the loaded object should be constructed with some additional data (e.g. all widget objects), this data is passed using arguments of Deserialize.

When loading **root objects** (e.g. scene, pallete), which are usually already present in the program, then Deserialize takes a pointer to already existing object and replaces its data with contents of file.

auto-conversions are possible when loading a file. This way, **PhysicalBody** can be converted to **PalleteItem** named `*Unnamed*` containing this body, and **PalleteItem** can be converted to pallete containing this only item. This is hadled by Deserialize function if the type of an object in file does not match expected type
