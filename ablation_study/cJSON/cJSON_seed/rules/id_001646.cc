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
//<ID> 1646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = NULL;
    cJSON *numbers = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *arr_ref = NULL;
    cJSON *fetched = NULL;
    char *out = NULL;

    root = cJSON_CreateObject();
    numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(numbers, num1);
    num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(numbers, num2);
    num3 = cJSON_CreateNumber(-3.14);
    cJSON_AddItemToArray(numbers, num3);
    arr_ref = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_ref", arr_ref);

    fetched = cJSON_GetObjectItem(root, "numbers");
    out = cJSON_PrintUnformatted(root);

    cJSON_free(out);
    cJSON_Delete(root);

    return 66;
}