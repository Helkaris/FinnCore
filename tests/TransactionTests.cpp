#include "TransactionTests.hpp"

const std::vector<std::pair<double, double>> TransactionAmounts = {{1.5678, 1.57},
                                                                   {-1.5678, -1.57},
                                                                   {1.566, 1.57},
                                                                   {-1.566, -1.57},
                                                                   {1.565, 1.57},
                                                                   {-1.565, -1.57},
                                                                   {1.564, 1.56},
                                                                   {-1.564, -1.56},
                                                                   {1.56, 1.56},
                                                                   {-1.56, -1.56},
                                                                   {1.5, 1.50},
                                                                   {-1.5, -1.50}};

class RoundAmountTests : public testing::TestWithParam<std::pair<double, double>>
{
};

TEST(TransactionTests, CreateWithWalletCategoryDateAndAmount)
{
    auto wallet = createWallet();
    auto category = createCategory();
    auto trans = createTransaction(DefaultAmount, wallet, category, DefaultCreationDT);

    ASSERT_EQ(trans->id(), DefaultId);
    ASSERT_EQ(trans->amount(), DefaultAmount);
    ASSERT_EQ(trans->type(), Transaction::Type::Income);
    ASSERT_EQ(trans->dateTime(), DefaultCreationDT);
    ASSERT_EQ(trans->date(), DefaultCreationDate);
    ASSERT_EQ(trans->time(), DefaultCreationTime);
    ASSERT_EQ(trans->wallet().lock(), wallet);
    ASSERT_EQ(trans->category().lock(), category);
    ASSERT_EQ(trans->description(), std::string{});
}

TEST(TransactionTests, CreateWithEmptyWalletShouldThrowException)
{
    ASSERT_THROW(createTransaction(DefaultAmount, {}, createCategory(), DefaultCreationDT), NullEntityError<Wallet>);
}

TEST(TransactionTests, CreateWithEmptyCategoryShouldThrowException)
{
    ASSERT_THROW(createTransaction(DefaultAmount, createWallet(), {}, DefaultCreationDT), NullEntityError<Category>);
}

TEST(TransactionTests, CreateWithZeroAmountShouldThrowException)
{
    ASSERT_THROW(createTransaction(0, createWallet(), createCategory(), DefaultCreationDT),
                 Transaction::ZeroAmountError);
}

TEST(TransactionTests, CreateWithInvalidDtShouldThrowException)
{
    ASSERT_THROW(createTransaction(DefaultAmount, createWallet(), createCategory(), {}),
                 Transaction::InvalidDateTimeError);
}

TEST(TransactionTests, CreateWithPositiveAmountTypeShouldBeIncome)
{
    auto trans = createTransaction(1, createWallet(), createCategory(), DefaultCreationDT);

    ASSERT_EQ(trans->type(), Transaction::Type::Income);
}

TEST(TransactionTests, CreateWithNegativeAmountTypeShouldBeExpense)
{
    auto trans = createTransaction(-1, createWallet(), createCategory(), DefaultCreationDT);

    ASSERT_EQ(trans->type(), Transaction::Type::Expense);
}

TEST_P(RoundAmountTests, CreateAndRoundAmountTo2DecimalPlaces)
{
    auto trans = createTransaction(GetParam().first, createWallet(), createCategory(), DefaultCreationDT);

    ASSERT_EQ(trans->amount(), GetParam().second);
}

INSTANTIATE_TEST_CASE_P(CreateSuite, RoundAmountTests, testing::ValuesIn(TransactionAmounts));

TEST(TransactionTests, UpdateWithZeroAmountShouldThrowException)
{
    auto trans = createTransaction();

    ASSERT_THROW(trans->updateAmount(0), Transaction::ZeroAmountError);
}

TEST(TransactionTests, UpdateWithPositiveAmountTypeShouldBeIncome)
{
    auto trans = createTransaction();

    trans->updateAmount(1);

    ASSERT_EQ(trans->type(), Transaction::Type::Income);
}

TEST(TransactionTests, UpdateWithNegativeAmountTypeShouldBeExpense)
{
    auto trans = createTransaction();

    trans->updateAmount(-1);

    ASSERT_EQ(trans->type(), Transaction::Type::Expense);
}

TEST_P(RoundAmountTests, UpdateAndRoundAmountTo2DecimalPlaces)
{
    auto trans = createTransaction();

    trans->updateAmount(GetParam().first);

    ASSERT_EQ(trans->amount(), GetParam().second);
}

INSTANTIATE_TEST_CASE_P(UpdateSuite, RoundAmountTests, testing::ValuesIn(TransactionAmounts));

TEST(TransactionTests, UpdateWithInvalidDtShouldThrowException)
{
    auto trans = createTransaction();

    ASSERT_THROW(trans->updateDateTime({}), Transaction::InvalidDateTimeError);
}

TEST(TransactionTests, UpdateWithValidDt)
{
    Date date{2020, 1, 1};
    Time time{1, 0, 0};
    DateTime dt{date, time};
    auto trans = createTransaction();

    trans->updateDateTime(dt);

    ASSERT_EQ(trans->dateTime(), dt);
    ASSERT_EQ(trans->date(), date);
    ASSERT_EQ(trans->time(), time);
}

TEST(TransactionTests, UpdateWithNewDescription)
{
    auto trans = createTransaction();
    std::string desr{"new descri[tion"};

    trans->updateDescription(desr);

    ASSERT_EQ(trans->description(), desr);
}
