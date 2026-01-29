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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_buf[] = "{\"names\":[\"one\",\"two\",\"three\"],\"status\":true}";
    const char *parse_end = NULL;
    size_t buf_len = sizeof(json_buf) - 1;
    cJSON *root = NULL;
    cJSON *names = NULL;
    cJSON *first_elem = NULL;
    cJSON *replacement_array = NULL;
    cJSON *new_false = NULL;
    char *printed = NULL;
    const char *const new_strings[] = { "alpha", "beta" };

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json_buf, buf_len, &parse_end, 1);
    names = cJSON_GetObjectItem(root, "names");
    first_elem = cJSON_GetArrayItem(names, 0);
    replacement_array = cJSON_CreateStringArray(new_strings, 2);
    new_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status_backup", new_false);
    cJSON_ReplaceItemInArray(names, 0, replacement_array);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}