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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *json_text = "{\"name\":\"initial\",\"items\":[1,2,3]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *raw_item = cJSON_CreateRaw("/* raw content */");
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *found_item = NULL;
    const char *found_str = NULL;
    size_t found_len = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw_entry", raw_item);
    cJSON_AddStringToObject(root, "added", "new_value");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    found_item = cJSON_GetObjectItem(root, "added");
    found_str = found_item ? cJSON_GetStringValue(found_item) : NULL;
    found_len = found_str ? strlen(found_str) : 0;
    memcpy(scratch + to_copy, found_str ? found_str : "", found_len);
    scratch[to_copy + found_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}