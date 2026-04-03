#include "../includes/lib3man.h"


Arena create_Arena(size_t arena_size){
    Arena arena = {0};
    arena.capacity = arena_size;
    arena.memory = malloc(arena.capacity);
    if(arena.memory == NULL){
        jsprintf("Error, Arena Allocation Failed\n");
        return arena;
    }
    arena.address = arena.memory;
    arena.cur_size = 0;
    return arena;
}

void *arena_Alloc(Arena * arena, size_t size){
    if(arena == NULL && size == 0){
        return NULL;
    }

    void * ptr = NULL;
    if(arena->memory == NULL) return NULL;
    if(arena->cur_size + size <= arena->capacity){
        ptr = arena->address;
        arena->address =  (char *)arena->address + size;
        arena->cur_size += size;
    }else{
        jsprintf("Error, Arena is Full\n");
    }
    return ptr;
}

void arena_reset(Arena * arena){
    // restart the arena writing 
    // it's ok we write over old data if we don't need it
    arena->address = arena->memory;
    arena->cur_size = 0;
}

void arena_free(Arena * arena){
    if(arena->memory == NULL) return;
    // free the arena before closing the programme
    free(arena->memory);
    arena->address = NULL;
    arena->capacity = 0;
    arena->cur_size = 0;
}

ArenaList *create_ArenaList(size_t size){
    ArenaList * arenaList = malloc(sizeof(ArenaList));
    if(arenaList == NULL){
        jsprintf("Error, ArenaList Allocation Failed\n");
        return NULL;
    }
    arenaList->next = NULL;
    if(arenaList == NULL) return NULL;
    *arenaList = (ArenaList){.arena = create_Arena(size), .next = NULL, .prev = NULL};
    return arenaList;
}

// linked list of arenas in case the first arena got full
//  so we can deallocat everything in the end
void *arenaList_Alloc(ArenaList **arenalist, size_t size){
    if((*arenalist)->arena.capacity >= (*arenalist)->arena.cur_size + size){
        return arena_Alloc(&(*arenalist)->arena, size);
    }
    ArenaList * prev = (*arenalist)->prev;
    
    while(prev != NULL){
        if(prev->arena.capacity >= prev->arena.cur_size + size){
            return arena_Alloc(&prev->arena, size);
        }
        prev = prev->prev;
    }

    // jsprintf("NEW ARENA CREATED IN THE ARENA-LIST\n");
    (*arenalist)->next = malloc(sizeof(ArenaList));

    if((*arenalist)->next == NULL){
        jsprintf("Error, ArenaList Allocation Failed\n");
        return NULL;
    }

    size_t capacity = (*arenalist)->arena.capacity;

    while(size > capacity){
        capacity *= 2;
    }
    
    prev = (*arenalist);
    *arenalist = (*arenalist)->next;
    (*arenalist)->prev = prev;
    (*arenalist)->arena = create_Arena(capacity);
    (*arenalist)->next = NULL;
    // jsprintf("------------arena: %d arena.capacity: %d arena.cur_size: %d------------------\n", &(*arenalist)->arena, (*arenalist)->arena.capacity, (*arenalist)->arena.cur_size);
    return arena_Alloc(&(*arenalist)->arena, size);
}

// TODO: THIS DOESN"T WORK CURRENTLY FIX IT 
void *arenaList_Realloc(ArenaList **arenalist, void *p, size_t oldsz , size_t newsz){
    // if(arenalist == NULL || p == NULL) return NULL;
    // if(newsz <= oldsz) return p;
    // size_t diff = newsz - oldsz;

    // if((char *)p + oldsz == (*arenalist)->arena.address && \
    //     (*arenalist)->arena.cur_size + diff <= (*arenalist)->arena.capacity)
    // {
    //     (*arenalist)->arena.address = (char *)(*arenalist)->arena.address + diff;
    //     (*arenalist)->arena.cur_size += diff;
    //     return p;
    // }else{
    //     void *temp = arenaList_Alloc(arenalist, newsz);
    //     memcpy(temp, p, oldsz);
    //     return temp;
    // }
    return NULL;
}

// free all the arenas we created
void arenaList_free(ArenaList * arenaList){
    while (arenaList != NULL) {
        jsprintf("+++++Arena freed address = %d+++++++++++\n", arenaList);
        ArenaList * temp = arenaList;
        arenaList = arenaList->prev;
        arena_free(&temp->arena);
        free(temp);
    }
}
