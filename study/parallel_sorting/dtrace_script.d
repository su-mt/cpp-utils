#!/usr/sbin/dtrace -s

#pragma D option quiet

dtrace:::BEGIN
{
    printf("Starting I/O tracing of the sort program...\n");
}

syscall::open*:entry
/execname == "mysort"/
{
    printf("[open]  %s\n", copyinstr(arg0));
}

syscall::read:entry
/execname == "mysort"/
{
    @reads[tid] = count();
}

syscall::write:entry
/execname == "mysort"/
{
    @writes[tid] = count();
}

dtrace:::END
{
    printa("Reads per thread:\n%@d\n", @reads);
    printa("Writes per thread:\n%@d\n", @writes);
}
