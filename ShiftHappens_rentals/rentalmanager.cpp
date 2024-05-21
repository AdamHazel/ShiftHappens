#include "rentalmanager.h"

bool rentalManager::checkRental(shiftDatabase &dataB, rental &checking)
{
    /*
     * 1. Check if table is empty
     *      If true, return safe confirm
     *  2. Check all records to see for any clashes in dates
     */
    bool safetoAdd = false;
    int count = dataB.rental_countRecords();
    if (count == 0)
    {
        safetoAdd = true;
        return safetoAdd;
    }
    /*
    if (count > 0)
    {
        rentals.clear();
        rentals = dataB.rental_FetchAll();
        for(const auto rental : rentals)
        {

        }
    }
*/
    return safetoAdd;
}
