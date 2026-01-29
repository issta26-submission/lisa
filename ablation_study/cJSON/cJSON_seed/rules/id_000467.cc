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
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *text = cJSON_CreateString("initial");
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *pc = NULL;
    cJSON *pnum = NULL;
    cJSON *ptext = NULL;
    cJSON *pflag = NULL;
    double val = 0.0;
    const char *s = NULL;
    int flag_int = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "text", text);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddTrueToObject(root, "flag");
    cJSON *newtext = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(child, "text", newtext);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    pc = cJSON_GetObjectItem(parsed, "child");
    pnum = cJSON_GetObjectItem(pc, "num");
    ptext = cJSON_GetObjectItem(pc, "text");
    pflag = cJSON_GetObjectItem(parsed, "flag");
    val = cJSON_GetNumberValue(pnum);
    s = cJSON_GetStringValue(ptext);
    flag_int = (int)cJSON_IsTrue(pflag);
    buffer[0] = (char)('0' + ((int)val % 10));
    buffer[1] = s ? s[0] : 'x';
    buffer[2] = (char)('0' + (flag_int % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}