#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Account.h"
#include "Transaction.h"
#include <iostream>

using ::testing::Return;
using ::testing::Ref;

class MockAccount : public Account {
public:
	MockAccount(int a, int b) : Account(a,b) {}
	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int diff), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};
class MockTransaction : public Transaction {
public:
	MockTransaction() : Transaction() {}
	MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};
TEST(Account, All_virt_func) {
	//Arrange
	MockAccount Mk(0, 0);
	EXPECT_CALL(Mk, GetBalance())
	.Times(1)
	.WillOnce(Return(33));
	EXPECT_CALL(Mk, ChangeBalance(1))
	.Times(1);
	EXPECT_CALL(Mk, Lock())
	.Times(1);
	EXPECT_CALL(Mk, Unlock())
	.Times(1);
	//Act
	int a = Mk.GetBalance();
	Mk.ChangeBalance(1);
	Mk.Lock();
	Mk.Unlock();
	//Assert
	EXPECT_EQ(a, 33);
}
TEST(Transaction, All_virt_func) {
	Account Mk(2, 3);
	Account Mk1(3, 4);
	MockTransaction Ts;
	EXPECT_CALL(Ts, SaveToDataBase(Ref(Mk), Ref(Mk1), 3))
	.Times(1);
	Ts.SaveToDataBase(Mk, Mk1, 3);
}
TEST(Transaction, Default_Func) {
	Account Mk(103, 3000);
	Account Mk1(214, 4000);
	Transaction a;
	EXPECT_EQ(a.Make(Mk, Mk1, 500), true);
	EXPECT_EQ(a.Make(Mk, Mk1, 500), true);
	EXPECT_ANY_THROW(a.Make(Mk1, Mk1, 500));
	EXPECT_ANY_THROW(a.Make(Mk, Mk1, -500));
	EXPECT_ANY_THROW(a.Make(Mk, Mk1, 1));
	a.set_fee(5000);
	EXPECT_EQ(a.Make(Mk, Mk1, 500), false);
	a.set_fee(1000);
	EXPECT_EQ(a.Make(Mk, Mk1, 2500), true);

}
int main(int argc, char **argv) {

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
