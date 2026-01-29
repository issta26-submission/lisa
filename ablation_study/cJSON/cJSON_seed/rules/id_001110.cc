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
//<ID> 1110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"name\":\"example\",\"items\":[\"one\",\"two\",\"three\"]}";
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *name_item = NULL;
    cJSON *second = NULL;
    int items_count = 0;
    char *name_str = NULL;
    char *second_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);
    items = cJSON_GetObjectItem(root, "items");
    name_item = cJSON_GetObjectItem(root, "name");

    // step 3: Operate / Validate
    items_count = cJSON_GetArraySize(items);
    second = cJSON_GetArrayItem(items, 1);
    second_str = cJSON_GetStringValue(second);
    name_str = cJSON_GetStringValue(name_item);

    // step 4: Cleanup
    (void)items_count;
    (void)name_str;
    (void)second_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}