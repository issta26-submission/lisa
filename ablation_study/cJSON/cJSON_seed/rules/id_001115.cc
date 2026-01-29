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
//<ID> 1115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *list = NULL;
    cJSON *name_item = NULL;
    cJSON *first_elem = NULL;
    char *name_str = NULL;
    char *first_str = NULL;
    char *printed = NULL;
    int list_size = 0;
    const char *json = "{\"name\":\"example\",\"list\":[\"one\",\"two\",\"three\"],\"nested\":{\"key\":\"value\"}}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    list = cJSON_GetObjectItem(root, "list");
    name_item = cJSON_GetObjectItem(root, "name");

    // step 3: Operate / Validate
    list_size = cJSON_GetArraySize(list);
    first_elem = cJSON_GetArrayItem(list, 0);
    name_str = cJSON_GetStringValue(name_item);
    first_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}