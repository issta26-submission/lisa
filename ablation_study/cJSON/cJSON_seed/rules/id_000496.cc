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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *retr_meta = NULL;
    cJSON *retr_true = NULL;
    cJSON *retr_arr_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "active", true_item);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    retr_meta = cJSON_GetObjectItem(root, "meta");
    retr_true = cJSON_GetObjectItem(retr_meta, "active");
    retr_arr_item = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items"), 1);
    buffer[0] = (char)('0' + (int)(cJSON_IsTrue(retr_true) * 1));
    buffer[1] = '\0';
    buffer[2] = (char)('0' + (int)cJSON_GetNumberValue(retr_arr_item));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}