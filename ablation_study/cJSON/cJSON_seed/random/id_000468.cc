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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char json_buffer[] = "  { \"num\" : 42.5 , \"txt\" : \" hello world \" }  ";
    char *printed = (char *)0;
    double extracted = 0.0;
    cJSON_bool has_num = 0;

    // step 2: Initialize
    cJSON_Minify(json_buffer);
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(json_buffer);
    num_item = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "data", str_item);
    cJSON_AddItemToObject(root, "num", num_item);

    // step 4: Operate
    extracted = cJSON_GetNumberValue((const cJSON *const)num_item);
    has_num = cJSON_HasObjectItem((const cJSON *)root, "num");

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)extracted;
    (void)has_num;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}