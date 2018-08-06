class test_class {
public:
    test_class();
    void _print();
};

extern "C" {
    test_class print();
}