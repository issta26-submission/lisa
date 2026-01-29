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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *direct_num = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_first = (cJSON *)0;
    cJSON *fetched_direct = (cJSON *)0;
    char *printed = (char *)0;
    double val_first = 0.0;
    double val_direct = 0.0;
    int validation = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate array with number items and add a direct number to root
    num_item1 = cJSON_CreateNumber(3.14);
    num_item2 = cJSON_CreateNumber(1.618);
    cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToArray(arr, num_item2);
    direct_num = cJSON_AddNumberToObject(root, "direct", 42.0);

    // step 4: Operate - attach array to root using CS API and serialize unformatted
    cJSON_AddItemToObjectCS(root, "numbers", arr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate and Cleanup - retrieve values, compute a simple validation, free serialized string and delete root
    fetched_arr = cJSON_GetObjectItem(root, "numbers");
    fetched_first = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_direct = cJSON_GetObjectItem(root, "direct");
    val_first = cJSON_GetNumberValue(fetched_first);
    val_direct = cJSON_GetNumberValue(fetched_direct);
    validation = (int)(val_first + val_direct);
    (void)validation;
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}