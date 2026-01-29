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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddNumberToObject(root, "id", 123.0);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_value = (char *)cJSON_GetStringValue(name_item);
    size_t name_len = strlen(name_value) + 1;
    char *name_copy = (char *)cJSON_malloc(name_len);
    memset(name_copy, 0, name_len);
    strcpy(name_copy, name_value);

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int pre_len = 512;
    char *prebuffer = (char *)cJSON_malloc(pre_len);
    memset(prebuffer, 0, pre_len);
    cJSON_bool preallocated_ok = cJSON_PrintPreallocated(root, prebuffer, pre_len, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 4: Validate & Cleanup
    const char *post_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    (void)post_name;
    (void)preallocated_ok;
    cJSON_free(buffered);
    cJSON_free(prebuffer);
    cJSON_free(name_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}