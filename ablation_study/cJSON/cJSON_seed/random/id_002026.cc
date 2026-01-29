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
//<ID> 2026
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
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *pi_item = NULL;
    cJSON *name_item = NULL;
    char *printed = NULL;
    cJSON *got = NULL;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Setup (create root, array and numeric items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateNumber(1.25);
    elem1 = cJSON_CreateNumber(2.5);

    // step 3: Configure (populate array and object with numbers and a string)
    added0 = cJSON_AddItemToArray(arr, elem0);
    added1 = cJSON_AddItemToArray(arr, elem1);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    name_item = cJSON_AddStringToObject(root, "name", "cJSON");

    // step 4: Operate (retrieve from array and extract numeric value)
    got = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(elem0 != NULL);
    validation ^= (int)(elem1 != NULL);
    validation ^= (int)(added0 != 0);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(pi_item != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(extracted_value == 2.5);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}