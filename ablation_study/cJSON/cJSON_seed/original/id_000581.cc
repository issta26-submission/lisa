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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"person\" : { \"name\" : \"Alice\", \"age\" : 30 }, \"tags\" : [ \"friend\", \"colleague\" ] } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    cJSON *tags = cJSON_GetObjectItem(root, "tags");
    cJSON *first_tag = cJSON_GetArrayItem(tags, 0);

    // step 2: Configure
    cJSON *person_dup = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", person_dup);
    cJSON *dup_name_item = cJSON_GetObjectItem(person_dup, "name");
    char *old_dup_name = cJSON_SetValuestring(dup_name_item, "Alicia");

    // step 3: Operate
    char *orig_name_str = cJSON_GetStringValue(name_item);
    char *dup_name_str = cJSON_GetStringValue(dup_name_item);
    char *tag0_str = cJSON_GetStringValue(first_tag);
    char *buffered = cJSON_PrintBuffered(root, 512, 1);
    int prealloc_len = 1024;
    char *prealloc_buf = (char *)cJSON_malloc(prealloc_len);
    memset(prealloc_buf, 0, prealloc_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prealloc_buf, prealloc_len, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "person_copy");
    (void)orig_name_str;
    (void)dup_name_str;
    (void)tag0_str;
    (void)old_dup_name;
    (void)pre_ok;
    cJSON_free(buffered);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}