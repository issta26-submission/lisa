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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON *value = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "value", value);

    // step 2: Configure & Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t len = strlen(printed) + 1;
    char *minbuf = (char*)cJSON_malloc(len);
    memset(minbuf, 0, len);
    memcpy(minbuf, printed, len);
    cJSON_Minify(minbuf);
    cJSON *parsed = cJSON_Parse(minbuf);

    // step 3: Operate & Validate
    cJSON *pname = cJSON_GetObjectItem(parsed, "name");
    cJSON *pvalue = cJSON_GetObjectItem(parsed, "value");
    double num = cJSON_GetNumberValue(pvalue);
    char *sname = cJSON_GetStringValue(pname);
    cJSON *numcopy = cJSON_CreateNumber(num);
    cJSON_AddItemToObject(parsed, "num_copy", numcopy);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *equalflag = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal", equalflag);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_free(printed);
    cJSON_free(minbuf);

    // API sequence test completed successfully
    return 66;
}