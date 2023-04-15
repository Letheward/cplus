/* ==== List ==== */

// todo: validate

template<typename T>
struct List {

    struct Node {

        Node* prev;
        Node* next;
        T     value;

        inline void link(Node* node) {
            if (!node) return;
            next       = node;
            node->prev = this;
        }

        inline void unlink() {
            prev = NULL;
            next = NULL;
        }
        
        inline void detach() {
            
            if (prev) {
                prev->next = NULL;
                prev = NULL;
            }
            
            if (next) {
                next->prev = NULL;
                next = NULL;
            }
        }
    };

    Node* data;
    u64   size;
    u64   position;

    Node* start;
    Node* end;

    // this is a fixed size list node pool, so if you don't want to use calloc(), 
    // just ignore init() and deinit(), and supply your buffer to data and size.
 
    bool init(u64 init_size = 256) {

        auto p = (Node*) calloc(init_size, sizeof(Node));
        if (!p) return false;

        *this = {};
        this->data = p;
        this->size = init_size;

        return true;
    }

    void deinit() {
        free(data);
        *this = {};
    }

    void reset() {
        memset(data, 0, position * sizeof(Node));
        position = 0;
        start = NULL;
        end   = NULL;
    }

    Node* new_node(T value = {}) {

        u64 pos = position;
        if (pos >= size) return NULL;

        data[pos].value = value; // note: this depends on calloc() and reset()
        position++;

        return &data[pos];
    }

    Node* add_start(T value = {}) {
        start = new_node(value);
        end   = start;
        return start;
    }

    Node* add(T value = {}) {

        if (!start) return add_start(value);

        auto next = new_node(value);
        end->link(next);
        end = next;

        return next;
    }
    
    Node* add_before_start(T value = {}) {

        if (!start) return add_start(value);

        auto node = new_node(value);
        node->link(start);
        start = node;

        return node;
    }

    Node* pop() {

        if (!end) return NULL;

        auto out = end;
        end = out->prev;

        if (end) end->next = NULL;
        else     start     = NULL;
        
        out->unlink();

        return out;
    }

    Node* pop_from_start() {

        if (!start) return NULL;

        auto out = start;
        start = out->next;
        
        if (start) start->prev = NULL;
        else       end         = NULL;

        out->unlink();

        return out;
    }

    Node* find(T value) {
        for (auto it = start; it; it = it->next) {
            if (it->value == value) return it;
        }
        return NULL;
    }

    Node* find_node(Node* node) {
        if (!node) return NULL;
        for (auto it = start; it; it = it->next) {
            if (it == node) return it;
        }
        return NULL;
    }

    // note: node must be in list!
    Node* insert(Node* node, T value) {

        if (!node) return NULL;
        
        auto old = node->next;
        if (!old) {
            if (node != end) return NULL; // prevent outside nodes corrupt the list, this is not ideal, think more
            return add(value);
        }

        auto next = new_node(value);
        node->link(next);
        next->link(old);
        
        return next;
    }

    // note: node must be in list!
    Node* insert_before(Node* node, T value) {

        if (!node) return NULL;
        
        auto old = node->prev;
        if (!old) {

            if (node != start) return NULL; // prevent outside nodes corrupt the list, this is not ideal, think more

            auto prev = new_node(value);
            start = prev;
            prev->link(node);

            return prev;
        }

        auto next = new_node(value);
        old->link(next);
        next->link(node);
        
        return next;
    }

    bool remove(Node* node) {

        if (!node) return false;

        auto prev = node->prev;
        auto next = node->next;

        if (!prev && !next) return false;

        if (prev) {
            prev->next = next;
            if (node == end) end = prev;
        }
        
        if (next) {
            next->prev = prev;
            if (node == start) start = next;
        }

        node->unlink();

        return true;
    }
    
    
    template<typename P>
    void remove_all_matches(P p) {
    
        for (auto it = start; it; it = it->next) {
            
            while (it) {
                if (p(it->value)) {
                    auto next = it->next;
                    remove(it);
                    it = next;
                } else {
                    break;
                }
            }

            if (!it) break;
        }
    }




    /* ---- Iterators ---- */
    
    template<bool reverse = false, typename P>
    inline void for_value(P p) {
        if constexpr (!reverse) {
            for (auto it = start; it; it = it->next)  p(it->value);
        } else {
            for (auto it = end;   it; it = it->prev)  p(it->value);
        }
    }
    
    template<bool reverse = false, typename P>
    inline void for_value_with_index(P p) {
   
        u64 index = 0;

        if constexpr (!reverse) {
            
            for (auto it = start; it; it = it->next) {
                p(it->value, index);
                index++;
            }
        
        } else {
            
            for (auto it = end; it; it = it->prev) {
                p(it->value, index);
                index++;
            }
        }
    }
    
    template<bool reverse = false, typename P>
    inline void for_node(P p) {
        if constexpr (!reverse) {
            for (auto it = start; it; it = it->next)  p(it);
        } else {
            for (auto it = end;   it; it = it->prev)  p(it);
        }
    }
    
    template<bool reverse = false, typename P>
    inline void for_node_with_index(P p) {
        
        u64 index = 0;
        
        if constexpr (!reverse) {
            
            for (auto it = start; it; it = it->next) {
                p(it, index);
                index++;
            }
        
        } else {
            
            for (auto it = end; it; it = it->prev) {
                p(it);
                index++;
            }
        }
    }
};


