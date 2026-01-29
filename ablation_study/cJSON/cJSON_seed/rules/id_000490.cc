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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *retr_items = NULL;
    cJSON *first_item = NULL;
    cJSON *retr_true = NULL;
    char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(meta, "note", "ready");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate / Validate
    retr_items = cJSON_GetObjectItem(root, "items");
    first_item = cJSON_GetArrayItem(retr_items, 0);
    first_str = cJSON_GetStringValue(first_item);
    retr_true = cJSON_GetObjectItem(root, "active");
    buffer[0] = (first_str != NULL) ? first_str[0] : 'x';
    buffer[1] = (char)('0' + (cJSON_IsTrue(retr_true) ? 1 : 0));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}