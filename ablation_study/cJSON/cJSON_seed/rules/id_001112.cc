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
//<ID> 1112
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
    cJSON *title_item = NULL;
    cJSON *first_item = NULL;
    char *title_str = NULL;
    char *first_str = NULL;
    int arr_size = 0;
    const char *json_text = "{\"title\":\"Example\",\"list\":[\"alpha\",\"beta\",\"gamma\"],\"meta\":{\"desc\":\"sample\"}}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);

    // step 3: Operate / Validate
    list = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(list);
    first_item = cJSON_GetArrayItem(list, 0);
    first_str = cJSON_GetStringValue(first_item);
    title_item = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(title_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}