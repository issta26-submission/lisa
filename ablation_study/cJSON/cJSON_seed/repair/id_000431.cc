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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"meta\": {\"author\": \"bob\", \"enabled\": true}, \"count\": 3}";
    cJSON *root = cJSON_Parse(json);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *author_item = cJSON_GetObjectItem(meta, "author");
    const char *author_str = cJSON_GetStringValue(author_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *meta_ref = cJSON_CreateObjectReference(meta);
    cJSON *container = cJSON_AddObjectToObject(root, "added_container");
    cJSON_AddItemToObject(container, "meta_ref", meta_ref);

    // step 3: Operate
    cJSON_bool has_container = cJSON_HasObjectItem(root, "added_container");
    cJSON_AddNumberToObject(root, "has_added_flag", (double)has_container);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON *added = cJSON_GetObjectItem(root, "added_container");
    cJSON *meta_ref_from_added = cJSON_GetObjectItem(added, "meta_ref");
    cJSON *author_from_ref = cJSON_GetObjectItem(meta_ref_from_added, "author");
    const char *author_back = cJSON_GetStringValue(author_from_ref);
    cJSON_AddStringToObject(root, "extracted_author", author_back);
    (void)count;
    (void)author_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}