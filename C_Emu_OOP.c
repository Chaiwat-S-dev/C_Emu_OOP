#include <stdio.h>

/* an object points to its class... */
struct Animal {
    const struct AnimalClass * classes;
};

/* which contains the virtual function Animal.Eat */
struct AnimalClass {
    void (*Eat)(struct Animal *); // Function pointer C famillar 'virtual' function C++
    void (*Move)(struct Animal *);
};

/* Since Animal.Move is not a virtual function
   it is not in the structure above. */
void Move(struct Animal * self)
{
	const struct AnimalClass * classes = *(const void **) self;
	if((*classes).Move)
		(*classes).Move(self);
	else
    	printf("<Animal at %p> moved in some way\n", (void *) self);
}

void _Move_Wolf(struct Animal * self)
{
    printf("<Wolf at %p> moved in some way\n", (void *) self);
}
/* unlike Move, which executes Animal.Move directly,
   Eat cannot know which function (if any) to call at compile time.
   Animal.Eat can only be resolved at run time when Eat is called. */
void Eat(struct Animal * self)
{
    const struct AnimalClass * classes = *(const void **) self;
    if (classes->Eat) 
        classes->Eat(self); // execute Animal.Eat
    else
        fprintf(stderr, "Eat not implemented\n");
}

/* implementation of Llama.Eat this is the target function 
   to be called by 'void Eat(struct Animal *).' */
static void _Llama_eat(struct Animal * self)
{
    printf("<Llama at %p> Llama's eat grass!\n", (void *) self);    
}

/* initialize class */
const struct AnimalClass Animal = {(void *) 0,_Move_Wolf}; // base class does not implement Animal.Eat
const struct AnimalClass Llama = {_Llama_eat,NULL};  // but the derived class does

int main(void)
{
   /* init objects as instance of its class */
   struct Animal animal = {& Animal};
   struct Animal llama = {& Llama};
   Move(& animal); // Animal.Move
   Move(& llama);  // Llama.Move
   Eat(& animal);  // cannot resolve Animal.Eat so print "Not Implemented" to stderr
   Eat(& llama);   // resolves Llama.Eat and executes
   return 0;
}
