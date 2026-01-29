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
//<ID> 1114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"meta\":\"v1\",\"data\":[{\"name\":\"Alice\",\"age\":30},{\"name\":\"Bob\",\"age\":25}],\"note\":\"example\"}";
    cJSON *root = NULL;
    cJSON *data_arr = NULL;
    cJSON *first_elem = NULL;
    cJSON *name_item = NULL;
    cJSON *meta_item = NULL;
    const char *name_str = NULL;
    const char *meta_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);

    // step 3: Operate / Validate
    data_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(data_arr);
    first_elem = cJSON_GetArrayItem(data_arr, 0);
    name_item = cJSON_GetObjectItem(first_elem, "name");
    name_str = cJSON_GetStringValue(name_item);
    meta_item = cJSON_GetObjectItem(root, "meta");
    meta_str = cJSON_GetStringValue(meta_item);

    // step 4: Cleanup
    (void)name_str;
    (void)meta_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}