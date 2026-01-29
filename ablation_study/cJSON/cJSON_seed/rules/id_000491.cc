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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_items = NULL;
    cJSON *first_elem = NULL;
    cJSON *retr_meta = NULL;
    cJSON *greet_item = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "ok", true_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_items = cJSON_GetObjectItem(parsed, "items");
    first_elem = cJSON_GetArrayItem(retr_items, 0);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(retr_meta, "greet");
    arr_size = cJSON_GetArraySize(retr_items);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';
    (void)first_elem;
    (void)greet_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}