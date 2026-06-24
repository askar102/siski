func lol() -> int:
    let count = 0;

    label loop:
        if (count >= 5):
            goto stop;
        count = count + 1;
        goto loop;
    
    label stop:
        return count;
