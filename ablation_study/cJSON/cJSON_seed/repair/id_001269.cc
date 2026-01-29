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
//<ID> 1269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"tags\":[\"a\",\"b\",\"c\"]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *tags = cJSON_GetObjectItem(root, "tags");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *meta_note = cJSON_CreateString("created by test");
    cJSON_AddItemToObject(meta, "note", meta_note);
    cJSON *new_tag = cJSON_CreateString("newtag");
    cJSON_AddItemToObject(root, "extra_tag", new_tag);

    // step 3: Operate and Validate
    int tag_count = cJSON_GetArraySize(tags);
    char count_buf[32];
    sprintf(count_buf, "%d", tag_count);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "tag_count_str", count_str);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}