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
//<ID> 567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"items\": [ { \"id\": 1, \"name\": \"orig\" } ], \"count\": 1 } ";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    char *new_name = cJSON_SetValuestring(name_item, "modified");
    cJSON *refs = cJSON_CreateArray();
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(refs, first_item);
    cJSON_AddItemToObject(root, "refs", refs);

    // step 3: Operate
    cJSON_bool has_count = cJSON_HasObjectItem(root, "count");
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *scratch = (char *)cJSON_malloc(printed_len + 1);
    memset(scratch, 0, printed_len + 1);
    if (printed) { strcpy(scratch, printed); }

    // step 4: Validate & Cleanup
    const char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    (void)new_name;
    (void)ref_added;
    (void)has_count;
    (void)extracted_name;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}