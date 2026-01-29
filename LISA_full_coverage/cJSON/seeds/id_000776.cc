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
//<ID> 776
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_meta = "{\"version\":1,\"note\":\"raw metadata\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddRawToObject(root, "metadata", raw_meta);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *fetched_array = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON_AddStringToObject(root, "label", "sample");

    // step 3: Operate
    cJSON *detached_first = cJSON_DetachItemFromArray(fetched_array, 0);
    cJSON_AddItemToObject(root, "first_detached", detached_first);
    char *printed = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    size_t printed_len = printed ? (size_t)strlen(printed) : 0;
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);

    // step 4: Validate & Cleanup
    cJSON *meta_from_parsed = cJSON_GetObjectItemCaseSensitive(parsed, "metadata");
    (void)meta_from_parsed;
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}