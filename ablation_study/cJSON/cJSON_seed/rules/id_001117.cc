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
//<ID> 1117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"list\": [\"one\", \"two\"], \"meta\": {\"name\":\"example\"}}";
    cJSON *root = NULL;
    cJSON *list = NULL;
    cJSON *first_elem = NULL;
    cJSON *meta = NULL;
    cJSON *name_obj = NULL;
    char *first_str = NULL;
    char *name_str = NULL;
    int list_size = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);
    list = cJSON_GetObjectItem(root, "list");
    meta = cJSON_GetObjectItem(root, "meta");

    // step 3: Operate / Validate
    list_size = cJSON_GetArraySize(list);
    first_elem = cJSON_GetArrayItem(list, 0);
    first_str = cJSON_GetStringValue(first_elem);
    name_obj = cJSON_GetObjectItem(meta, "name");
    name_str = cJSON_GetStringValue(name_obj);

    // step 4: Cleanup
    (void)first_str;
    (void)name_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}