#include <atomic>
#include <vector>

template <typename T>
class MPMCQueue {
  struct Node {
    T data;
    std::atomic<size_t> sequence;
  };

  std::vector<Node> buffer;
  std::atomic<size_t> head;
  std::atomic<size_t> tail;

public:
  MPMCQueue(size_t size) : buffer(size) {
    head = tail = 0;
    for (size_t i = 0; i < size; i++) {
        buffer[i].sequence.store(i);
    }
  }

  bool push(T data) {
    size_t tail_index = tail.load();
    Node& tail_node = buffer[tail_index];
    size_t next_sequence = tail_node.sequence.load() + 1;
    if (next_sequence - head.load() > buffer.size()) {
        return false;
    }
    tail_node.data = data;
    tail_node.sequence.store(next_sequence);
    tail.store(tail_index + 1);
    return true;
  }

  T pop() {
    T result;
    size_t head_index = head.load();
    Node& head_node = buffer[head_index];
    if (head_node.sequence.load() == head_index) {
        return -1;
    }
    result = head_node.data;
    head_node.sequence.store(head_index + buffer.size());
    head.store(head_index + 1);
    return result;
  }
};