#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
    int hcount;

    struct condition done;
    struct condition hin;

	struct lock lock;
};

void
reaction_init(struct reaction *reaction)
{

    reaction->hcount = 0;

    cond_init(&reaction->done);
    cond_init(&reaction->hin);

	lock_init(&reaction->lock);
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&reaction->lock);
    reaction->hcount++;

    cond_signal(&reaction->hin, &reaction->lock);
    cond_wait(&reaction->done, &reaction->lock);

    lock_release(&reaction->lock);
}

void
reaction_o(struct reaction *reaction)
{
    lock_acquire(&reaction->lock);

    while(reaction->hcount < 2)
        cond_wait(&reaction->hin, &reaction->lock);

    reaction->hcount -= 2;
    make_water();

    cond_signal(&reaction->done, &reaction->lock);
    cond_signal(&reaction->done, &reaction->lock);

    lock_release(&reaction->lock);
}
