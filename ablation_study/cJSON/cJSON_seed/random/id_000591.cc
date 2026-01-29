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
//<ID> 591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"name\":\"root\",\"items\":[\"one\",\"two\",\"three\"],\"value\":1}";
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *retr_new_num = (cJSON *)0;
    char *first_str = (char *)0;
    int items_count = 0;

    // step 2: Initialize
    root = cJSON_Parse(json);

    // step 3: Configure
    items = cJSON_GetObjectItem(root, "items");
    items_count = cJSON_GetArraySize(items);
    first_item = cJSON_GetArrayItem(items, 0);
    first_str = cJSON_GetStringValue(first_item);
    retr_new_num = cJSON_AddNumberToObject(root, "new_number", 42.0);

    // step 4: Operate
    (void)first_str;
    (void)retr_new_num;

    // step 5: Validate
    (void)items_count;
    (void)first_item;
    (void)items;
    (void)json;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}