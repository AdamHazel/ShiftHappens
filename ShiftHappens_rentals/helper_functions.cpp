#include "helper_functions.h"

/**
 * @brief Calculates total price for a given rental
 * @param QDate     start date
 * @param QDate     end date
 * @param uint      dayPrice
 * @return
 */
uint totalPriceCalculate(QDate start, QDate end, uint dayPrice)
{
    uint dayCount { 1 };
    while (start != end)
    {
        start = start.addDays(1);
        ++dayCount;
    }

    uint totalPrice = dayCount * dayPrice;

    return totalPrice;
}
