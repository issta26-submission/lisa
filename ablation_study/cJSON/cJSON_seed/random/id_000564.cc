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
//<ID> 564
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON *added_count = (cJSON *)0;
    double count_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(7.0);
    num2 = cJSON_CreateNumber(13.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    (void)cJSON_AddItemToObject(root, "numbers", arr);
    added_count = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)added_count;
    (void)count_value;
    (void)printed;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}