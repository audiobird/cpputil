#include "doctest.h"
#include "util/static_polymorphism.hh"

TEST_CASE("Static Polymorphic Default") {
	struct Base {
		virtual int get() = 0;
	};
	struct D0 : Base {
		virtual int get() override {
			return 0;
		}
	};
	struct D1 : Base {
		int a;
		D1(int a)
			: a{a} {
		}
		virtual int get() override {
			return a;
		}
	};

	StaticPolymorphicContainer<Base, D0, D1> abstract;

	abstract.transform<D0>();
	CHECK(abstract->get() == 0);

	abstract.transform<D1>(0xBEEF);
	CHECK(abstract->get() == 0xBEEF);
}
