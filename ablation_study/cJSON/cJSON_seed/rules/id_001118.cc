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
//<ID> 1118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *name_item = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    const char *json = "{\"name\":\"example\",\"items\":[\"one\",\"two\",\"three\"]}";
    int arr_size = 0;
    const char *name_str = NULL;
    const char *first_str = NULL;

    // step 2: Setup / Parse
    root = cJSON_Parse(json);
    name_item = cJSON_GetObjectItem(root, "name");
    items = cJSON_GetObjectItem(root, "items");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(items);
    first_item = cJSON_GetArrayItem(items, 0);
    name_str = cJSON_GetStringValue(name_item);
    first_str = cJSON_GetStringValue(first_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}