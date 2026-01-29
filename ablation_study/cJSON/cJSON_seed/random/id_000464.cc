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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *subobj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_num = (cJSON *)0;
    char *printed = (char *)0;
    char json_buf[] = " { \"num\" :  42.5 , \"text\" : \"hello world\" } ";
    double val_from_parsed = 0.0;
    double val_from_constructed = 0.0;
    double combined = 0.0;
    cJSON_bool has_num = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    subobj = cJSON_CreateObject();
    str_item = cJSON_CreateString("constructed");
    num_item = cJSON_CreateNumber(10.0);

    // step 3: Configure
    cJSON_AddItemToObject(subobj, "text", str_item);
    cJSON_AddItemToObject(subobj, "num", num_item);
    cJSON_AddItemToObject(root, "data", subobj);
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse((const char *)json_buf);

    // step 4: Operate
    has_num = cJSON_HasObjectItem(parsed, "num");
    parsed_num = cJSON_GetObjectItem(parsed, "num");
    val_from_parsed = cJSON_GetNumberValue((const cJSON *const)parsed_num);
    val_from_constructed = cJSON_GetNumberValue((const cJSON *const)num_item);
    combined = val_from_parsed + val_from_constructed;

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)has_num;
    (void)val_from_parsed;
    (void)val_from_constructed;
    (void)combined;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}