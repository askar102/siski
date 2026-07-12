/* This is example of loop in Siski */

func main() -> int {
    let var_example: int = 10;

    label loop:
        if (var_example >= 5) {
            goto stop;
        }
        var_example = var_example + 1;
        goto loop;

    label stop:
        return var_example;
}

%FUCK_COMPILER_FLAG
U0 example() {
    I64 lol = 10;
    if (lol == 10) {

    }
}


U0 resolve() 
{
    I64 num = 10;
    I64 ans = 10 + num * 30 
}

void resolve()
{
    // int64_t num = 10; remove it
    int64_t ans = 10 + 10 * 30
}