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
//<ID> 692
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    size_t buf_size = 256;
    char *scratch = (char *)cJSON_malloc(buf_size);
    memset(scratch, 0, buf_size);
    const char *json1 = "{\"name\":\"alpha\",\"count\":7}";
    const char *json2 = "{\"message\":\"hello\",\"active\":true}TRAIL";
    const char *end1 = NULL;
    const char *end2 = NULL;
    cJSON *parsed1 = cJSON_ParseWithOpts(json1, &end1, 1);
    cJSON *parsed2 = cJSON_ParseWithLengthOpts(json2, strlen(json2) - 5, &end2, 0);
    cJSON *root = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "first", parsed1);
    cJSON_AddItemToObject(root, "second", parsed2);
    cJSON_AddBoolToObject(root, "configured", 1);

    // step 3: Operate / Validate
    cJSON *first_obj = cJSON_GetObjectItem(root, "first");
    cJSON *name_item = cJSON_GetObjectItem(first_obj, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    (void)name_is_string;
    cJSON_PrintPreallocated(root, scratch, (int)buf_size, 0);
    size_t printed_len = strlen(scratch);
    (void)printed_len;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}