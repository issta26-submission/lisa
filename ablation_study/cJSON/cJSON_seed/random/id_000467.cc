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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    char json_buffer[] = " { \"num\" :  42 , \"greeting\" : \"hello\" } ";
    char *printed = (char *)0;
    cJSON_bool has_greeting = 0;
    double num_value = 0.0;

    // step 2: Initialize
    cJSON_Minify(json_buffer);
    root = cJSON_Parse((const char *)json_buffer);
    num_item = cJSON_GetObjectItem((const cJSON *const)root, "num");

    // step 3: Configure
    str_item = cJSON_CreateString("world");
    cJSON_ReplaceItemInObject(root, "greeting", str_item);

    // step 4: Operate
    num_value = cJSON_GetNumberValue((const cJSON *const)num_item);
    has_greeting = cJSON_HasObjectItem(root, "greeting");
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)num_value;
    (void)has_greeting;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}