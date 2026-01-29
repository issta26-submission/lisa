#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 2015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    char *printed = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation = 0;
    double val0 = 0.0;
    double val1 = 0.0;

    // step 2: Setup (create array and number elements)
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);

    // step 3: Configure (populate the array)
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate (retrieve items, read numbers, and print)
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(got0);
    val1 = cJSON_GetNumberValue(got1);
    printed = cJSON_PrintUnformatted(arr);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(val0 > 0.0);
    validation ^= (int)(val1 > 0.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}