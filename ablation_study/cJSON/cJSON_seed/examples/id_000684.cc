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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *str = cJSON_CreateString("example");
    cJSON *truth = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, num);
    cJSON_AddItemToArray(items, str);
    cJSON_AddItemToArray(items, truth);
    cJSON_AddItemToObject(root, "items", items);
    (void)version;

    // step 2: Operate (serialize -> copy -> minify -> parse)
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len_snapshot = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len_snapshot + 1);
    memset(buffer, 0, len_snapshot + 1);
    memcpy(buffer, snapshot, len_snapshot + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Validate (inspect parsed structure and compare)
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int parsed_count = cJSON_GetArraySize(parsed_items);
    cJSON *first = cJSON_GetArrayItem(parsed_items, 0);
    cJSON *second = cJSON_GetArrayItem(parsed_items, 1);
    cJSON_bool equal_arrays = cJSON_Compare(items, parsed_items, 1);
    char *parsed_items_unformatted = cJSON_PrintUnformatted(parsed_items);
    (void)first;
    (void)second;
    (void)parsed_count;
    (void)equal_arrays;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(parsed_items_unformatted);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}