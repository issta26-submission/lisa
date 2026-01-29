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
//<ID> 1127
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *num_total = (cJSON *)0;
    cJSON *fetched_total = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_total = 0.0;
    cJSON_bool added_cs = (cJSON_bool)0;
    cJSON_bool is_num = (cJSON_bool)0;

    // step 2: Setup - create root object and an array to hold numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_cs = cJSON_AddItemToObjectCS(root, "numbers", arr);

    // step 3: Configure - populate array and add a separate numeric member to root
    elem1 = cJSON_CreateNumber(1.0);
    elem2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    num_total = cJSON_AddNumberToObject(root, "total", 3.14159);

    // step 4: Operate & Validate - retrieve values and serialize unformatted
    fetched_total = cJSON_GetObjectItem(root, "total");
    is_num = cJSON_IsNumber(fetched_total);
    retrieved_total = cJSON_GetNumberValue(fetched_total);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    (void)added_cs;
    (void)is_num;
    (void)retrieved_total;
    (void)num_total;
    (void)elem1;
    (void)elem2;

    // API sequence test completed successfully
    return 66;
}