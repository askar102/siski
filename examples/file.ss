const an_func = #import("another_file.ss")::another_func;

# Just a example of function
public func anotherExample(arg1: int, arg2: int) -> int {
    exC {
        // it is typedef int32_t siski_int32_t
        siski_int32_t buffer[200];
        buffer[0] = 1;

        // it is not C construction anymore:
        export buffer[0] as first_argument: int; 
    };
    return first_argument + 1;
}


public func loopExample(input: *int, count: int) -> int {
    # input it is a pointer here!
    extern C {
        for (int i = 0; i <= count; ++i) {
            (*input)++;
        }

        export *input as new_input: int;
    };

    return new_input;
}


func main() -> int {
    my_input = 1;
    loopExample(&my_input, 10);
}


