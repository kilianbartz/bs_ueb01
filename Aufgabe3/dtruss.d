#!/usr/sbin/dtrace -s

sched:::off-cpu
/pid == $target/
{
    self->start = timestamp;
}

sched:::on-cpu
/self->start && pid == $target/
{
    @["Context Switch Time (ns)"] = avg(timestamp - self->start);
    self->start = 0;
}
