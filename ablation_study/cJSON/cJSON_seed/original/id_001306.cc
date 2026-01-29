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
//<ID> 1306
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
    cJSON *list = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(list, item0);
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(version));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "tag", cJSON_CreateStringReference("meta-tag"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *got_tag = cJSON_GetObjectItemCaseSensitive(got_meta, "tag");
    cJSON *got_list = cJSON_GetObjectItemCaseSensitive(root, "list");
    cJSON *second_item = cJSON_GetArrayItem(got_list, 1);
    char *second_val = cJSON_GetStringValue(second_item);
    cJSON *copied = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(meta, "copied", copied);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}