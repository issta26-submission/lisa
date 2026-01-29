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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *t = cJSON_CreateTrue();
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON *n2 = cJSON_CreateNumber(6.0);
    cJSON *retr_arr = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *elem3 = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    double val3 = 0.0;
    cJSON_bool is_t = 0;
    double total = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemReferenceToArray(arr, n2);

    // step 3: Operate / Validate
    retr_arr = cJSON_GetObjectItem(root, "items");
    elem0 = cJSON_GetArrayItem(retr_arr, 0);
    elem1 = cJSON_GetArrayItem(retr_arr, 1);
    elem2 = cJSON_GetArrayItem(retr_arr, 2);
    elem3 = cJSON_GetArrayItem(retr_arr, 3);
    is_t = cJSON_IsTrue(elem0);
    val1 = cJSON_GetNumberValue(elem1);
    val2 = cJSON_GetNumberValue(elem2);
    val3 = cJSON_GetNumberValue(elem3);
    total = val1 + val2 + val3 + (double)is_t;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}