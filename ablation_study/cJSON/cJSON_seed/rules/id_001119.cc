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
//<ID> 1119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *elem = NULL;
    cJSON *name_item = NULL;
    cJSON *title_item = NULL;
    char *name_str = NULL;
    char *title_str = NULL;
    int arr_size = 0;
    const char *json = "{\"title\":\"example\",\"items\":[{\"name\":\"first\"},{\"name\":\"second\"}],\"count\":2}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "items");
    title_item = cJSON_GetObjectItem(root, "title");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    elem = cJSON_GetArrayItem(arr, arr_size - 1);
    name_item = cJSON_GetObjectItem(elem, "name");
    name_str = cJSON_GetStringValue(name_item);
    title_str = cJSON_GetStringValue(title_item);

    // step 4: Cleanup
    (void)name_str;
    (void)title_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}