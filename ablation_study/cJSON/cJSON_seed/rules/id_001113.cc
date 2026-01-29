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
//<ID> 1113
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
    cJSON *greeting = NULL;
    cJSON *first_elem = NULL;
    char *printed = NULL;
    char *greeting_str = NULL;
    char *first_elem_str = NULL;
    int list_size = 0;
    const char json[] = "{\"greeting\":\"hello\",\"list\":[\"one\",\"two\",\"three\"]}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    list = cJSON_GetObjectItem(root, "list");
    greeting = cJSON_GetObjectItem(root, "greeting");

    // step 3: Operate / Validate
    list_size = cJSON_GetArraySize(list);
    first_elem = cJSON_GetArrayItem(list, 0);
    greeting_str = cJSON_GetStringValue(greeting);
    first_elem_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}