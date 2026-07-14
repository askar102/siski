int loop(int i) {
    loop:
        if (i > 10) {
            goto end_loop;
        }
        i = i + 1;
    goto loop;

    end_loop:
        return i;
}

// Example: struct Name { i32 lol; }
//[[MACRO_WITHOUT_MARK]]
//new_macro! struct() {
    //U0 __macro_struct(%name, %body) {
        //byte[100] _struct_name = %name;
        //...
    //}
//}

int main() {
    int res = loop(1);
    return res;
}

